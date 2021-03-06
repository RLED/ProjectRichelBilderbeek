#include "trianglemeshcellscreator.h"

#include <cassert>

#include "Shiny.h"

#include "trianglemeshcell.h"
#include "trianglemeshcellfactory.h"
#include "trianglemeshcellscreatorfactory.h"
#include "trianglemeshface.h"
#include "trianglemeshfacefactory.h"
#include "trianglemeshhelper.h"
#include "trianglemeshpoint.h"
#include "trianglemeshpointfactory.h"
#include "trianglemeshtemplate.h"
#include "trianglemeshcreateverticalfacesstrategies.h"
#include "trace.h"

ribi::trim::CellsCreator::CellsCreator(
  const boost::shared_ptr<const Template> t,
  const int n_layers,
  const boost::units::quantity<boost::units::si::length> layer_height,
  const CreateVerticalFacesStrategy strategy,
  const CellsCreatorFactory&
) : m_cells(CreateCells(t,n_layers,layer_height,strategy)),
    m_strategy{strategy}
{
  #ifndef NDEBUG
  Test();
  #endif
}

void ribi::trim::CellsCreator::CheckCells(const std::vector<boost::shared_ptr<Cell>>& cells) noexcept
{
  for (const auto cell: cells)
  {
    assert(cell);
    assert(cell->GetFaces().size() == 5 || cell->GetFaces().size() == 8);
  }
}

std::vector<boost::shared_ptr<ribi::trim::Cell>> ribi::trim::CellsCreator::CreateCells(
  const boost::shared_ptr<const Template> t,
  const int n_layers,
  const boost::units::quantity<boost::units::si::length> layer_height,
  const CreateVerticalFacesStrategy strategy
) noexcept
{
  PROFILE_FUNC();
  const std::vector<boost::shared_ptr<Point>> all_points {
    CreatePoints(t,n_layers,layer_height)
  };

  const std::vector<boost::shared_ptr<Face>> hor_faces {
    CreateHorizontalFaces(t,all_points,n_layers)
  };

  //ver_faces usused: cause of issue 168?
  std::vector<boost::shared_ptr<Face>> ver_faces(
    CreateVerticalFaces(t,all_points,n_layers,strategy)
  );
  assert(!ver_faces.empty()); //168 : perhaps ver_faces is removed by compiler?
  assert(ver_faces.size() > hor_faces.size()); //168 : perhaps ver_faces is removed by compiler?
  #ifndef NDEBUG
  for(auto f:ver_faces) { assert(f); }
  #endif

  const int n_hor_faces_per_layer = static_cast<int>(t->GetFaces().size());
  const int n_cells_per_layer = n_hor_faces_per_layer;

  std::vector<boost::shared_ptr<Cell>> cells;
  for (int layer=0; layer!=n_layers-1; ++layer) //-1 because there are no points above the top layer
  {
    std::clog << ".";
    for (int i=0; i!=n_cells_per_layer; ++i)
    {
      const int bottom_face_index = ((layer + 0) * n_hor_faces_per_layer) + i;
      const int top_face_index    = ((layer + 1) * n_hor_faces_per_layer) + i;
      assert(bottom_face_index >= 0);
      assert(top_face_index    >= 0);
      assert(bottom_face_index < static_cast<int>(hor_faces.size()));
      assert(top_face_index    < static_cast<int>(hor_faces.size()));
      const std::vector<boost::shared_ptr<Face>> these_ver_faces {
        FindKnownFacesBetween(
          hor_faces[bottom_face_index],
          hor_faces[top_face_index]
        )
      };

      if (strategy == CreateVerticalFacesStrategy::one_face_per_square )
      {
        assert(these_ver_faces.size() == 3);
        assert(hor_faces[bottom_face_index]);
        assert(hor_faces[top_face_index]);
        assert(these_ver_faces[0]);
        assert(these_ver_faces[1]);
        assert(these_ver_faces[2]);
        const boost::shared_ptr<Cell> cell(
          CellFactory().Create(
            {
              hor_faces[bottom_face_index],
              hor_faces[top_face_index],
              these_ver_faces[0],
              these_ver_faces[1],
              these_ver_faces[2]
            },
            strategy
          )
        );
        assert(hor_faces[bottom_face_index]);
        assert(hor_faces[top_face_index]);
        assert(Helper().IsHorizontal(*hor_faces[bottom_face_index]));
        assert(Helper().IsHorizontal(*hor_faces[top_face_index]));
        assert(Helper().IsVertical(*these_ver_faces[0]));
        assert(Helper().IsVertical(*these_ver_faces[1]));
        assert(Helper().IsVertical(*these_ver_faces[2]));

        cells.push_back(cell);
      }
      else
      {
        assert(these_ver_faces.size() == 6);
        const boost::shared_ptr<Cell> cell {
          CellFactory().Create(
            {
              hor_faces[bottom_face_index],
              hor_faces[top_face_index],
              these_ver_faces[0],
              these_ver_faces[1],
              these_ver_faces[2],
              these_ver_faces[3],
              these_ver_faces[4],
              these_ver_faces[5]
            },
            strategy
          )
        };
        assert(hor_faces[bottom_face_index]);
        assert(hor_faces[top_face_index]);
        assert(Helper().IsHorizontal(*hor_faces[bottom_face_index]));
        assert(Helper().IsHorizontal(*hor_faces[top_face_index]));
        assert(Helper().IsVertical(*these_ver_faces[0]));
        assert(Helper().IsVertical(*these_ver_faces[1]));
        assert(Helper().IsVertical(*these_ver_faces[2]));
        assert(Helper().IsVertical(*these_ver_faces[3]));
        assert(Helper().IsVertical(*these_ver_faces[4]));
        assert(Helper().IsVertical(*these_ver_faces[5]));

        cells.push_back(cell);
      }
    }
  }

  CheckCells(cells);
  ver_faces.clear();
  return cells;
}

std::vector<boost::shared_ptr<ribi::trim::Face>> ribi::trim::CellsCreator::CreateHorizontalFaces(
  const boost::shared_ptr<const Template> t,
  const std::vector<boost::shared_ptr<Point>>& all_points,
  const int n_layers
)
{
  PROFILE_FUNC();
  std::vector<boost::shared_ptr<Face> > v;
  assert(t);
  assert(!all_points.empty());

  const int n_points_per_layer = static_cast<int>(t->GetPoints().size());
  const int n_faces_per_layer = static_cast<int>(t->GetFaces().size());
  assert(n_layers > 0);
  if (n_layers < 2) { TRACE("WARNING: NEED AT LEAST TWO HORIZONTAL PLANES TO CREATE CELLS"); }
  v.reserve(n_layers * n_points_per_layer);

  for (int layer=0; layer!=n_layers; ++layer)
  {
    const int point_offset = n_points_per_layer * layer;
    for (const std::vector<int>& face_point_indices: t->GetFacePointIndices())
    {
      const int face_index = static_cast<int>(v.size());
      assert(face_point_indices.size() == 3); //Triangulation
      std::vector<boost::shared_ptr<Point>> face_points;
      for (int point_index: face_point_indices)
      {
        assert(point_index + point_offset < static_cast<int>(all_points.size()));
        face_points.push_back(all_points[point_index + point_offset]);
        #ifndef NDEBUG
        if (face_points.size() >= 2 && face_points[0]->CanGetZ())
        {
          assert(face_points.front()->GetZ() == face_points.back()->GetZ());
        }
        #endif
      }
      assert(layer == 0 || face_index - n_faces_per_layer >= 0);
      assert(layer == 0 || face_index - n_faces_per_layer < static_cast<int>(v.size()));
      if ( (layer % 2 == 0 && !Helper().IsClockwiseHorizontal(face_points))
        || (layer % 2 == 1 &&  Helper().IsClockwiseHorizontal(face_points))
      )
      {
        std::reverse(face_points.begin(),face_points.end());
      }

      if(!Helper().IsConvex(face_points)) { Helper().MakeConvex(face_points); }
      assert(Helper().IsConvex(face_points));

      const boost::shared_ptr<Face> face {
        FaceFactory().Create(
          face_points,
          FaceOrientation::horizontal
        )
      };
      v.push_back(face);
    }
  }
  return v;
}

std::vector<boost::shared_ptr<ribi::trim::Point> > ribi::trim::CellsCreator::CreatePoints(
  const boost::shared_ptr<const Template> t,
  const int n_layers,
  const boost::units::quantity<boost::units::si::length> layer_height
)
{
  PROFILE_FUNC();
  std::vector<boost::shared_ptr<Point> > v;

  TRACE(n_layers);

  for (int i=0; i!=n_layers; ++i)
  {
    for (const boost::shared_ptr<const Point> point: t->GetPoints())
    {
      const boost::shared_ptr<Point> new_point {
        PointFactory().Create(
          point->GetCoordinat()
        )
      };
      new_point->SetZ(static_cast<double>(i) * layer_height );
      v.push_back(new_point);
    }
  }
  assert(static_cast<int>(v.size()) == static_cast<int>(t->GetPoints().size()) * n_layers);

  return v;
}

std::vector<boost::shared_ptr<ribi::trim::Face>> ribi::trim::CellsCreator::CreateVerticalFaces(
  const boost::shared_ptr<const Template> t,
  const std::vector<boost::shared_ptr<Point>>& all_points,
  const int n_layers,
  const CreateVerticalFacesStrategy strategy
) noexcept
{
  PROFILE_FUNC();
  const std::vector<std::pair<int,int>> edges {
    t->GetEdges()
  };
  assert(!edges.empty());
  const int n_edges = static_cast<int>(edges.size());
  const int n_points_per_layer = static_cast<int>(t->GetPoints().size());
  const int n_ver_faces { strategy == CreateVerticalFacesStrategy::one_face_per_square
    ? 1 * n_edges
    : 2 * n_edges //For every horizontal edge, two triangles are used instead
  };

  std::vector<boost::shared_ptr<Face> > v;
  v.reserve(n_ver_faces * (n_layers - 1));

  assert(n_layers > 0);
  if (n_layers < 2) { TRACE("WARNING: NEED AT LEAST TWO HORIZONTAL PLANES TO CREATE CELLS"); }

  for (int layer=0; layer!=n_layers-1; ++layer) //-1 because there are no points above the top layer
  {
    const int points_offset = n_points_per_layer * layer;
    for (const std::pair<int,int>& edge: edges)
    {
      assert(edge.first < edge.second);

      assert(points_offset + edge.first  < static_cast<int>(all_points.size()));
      assert(points_offset + edge.second < static_cast<int>(all_points.size()));
      assert(points_offset + edge.first  + n_points_per_layer < static_cast<int>(all_points.size()));
      assert(points_offset + edge.second + n_points_per_layer < static_cast<int>(all_points.size()));
      if (strategy == CreateVerticalFacesStrategy::one_face_per_square)
      {
        //Ordering cannot be known for sure to be convex from these indices
        std::vector<boost::shared_ptr<Point>> face_points {
          all_points[points_offset + edge.first],
          all_points[points_offset + edge.second],
          all_points[points_offset + edge.first  + n_points_per_layer],
          all_points[points_offset + edge.second + n_points_per_layer]
        };

        #ifndef NDEBUG
        if (!Helper().IsConvex(face_points))
        {
          TRACE("NOT CONVEX");
          for (auto p: face_points) { assert(p); TRACE(*p); }
        }
        #endif

        //Order face_points
        if (!Helper().IsConvex(face_points)) { Helper().MakeConvex(face_points); }

        #ifndef NDEBUG
        if (!Helper().IsConvex(face_points))
        {
          TRACE("ERROR");
          for (auto p: face_points) { TRACE(*p); }
          TRACE("BREAK");
        }
        #endif

        assert(Helper().IsConvex(face_points));

        //Cannot order face winding yet, need Cells for this
        const boost::shared_ptr<Face> face {
          FaceFactory().Create(
            face_points,
            FaceOrientation::vertical
          )
        };
        v.push_back(face);
      }
      else
      {
        const std::vector<boost::shared_ptr<Point>> face_points_1 {
          all_points[points_offset + edge.first],
          all_points[points_offset + edge.second],
          all_points[points_offset + edge.first + n_points_per_layer]
        };
        assert(Helper().IsConvex(face_points_1)
          && "FaceFactory expects convex ordered points");

        //Cannot order face winding yet, need Cells for this
        const boost::shared_ptr<Face> face_1 {
          FaceFactory().Create(
            face_points_1,
            FaceOrientation::vertical
          )
        };
        v.push_back(face_1);

        std::vector<boost::shared_ptr<Point>> face_points_2 {
          all_points[points_offset + edge.second],
          all_points[points_offset + edge.second + n_points_per_layer],
          all_points[points_offset + edge.first + n_points_per_layer]
        };
        if (!Helper().IsConvex(face_points_2)) { Helper().MakeConvex(face_points_2); }

        assert(Helper().IsConvex(face_points_2)
          && "FaceFactory expects convex ordered points");

        const boost::shared_ptr<Face> face_2 {
          FaceFactory().Create(
            face_points_2,
            FaceOrientation::vertical
          )
        };
        v.push_back(face_2);
      }
    }
  }

  assert(n_ver_faces * (n_layers - 1) == static_cast<int>(v.size()));
  return v;
}

std::vector<boost::shared_ptr<ribi::trim::Face>> ribi::trim::CellsCreator::FindKnownFacesBetween(
  const boost::shared_ptr<const Face> a, const boost::shared_ptr<const Face> b
)
{
  PROFILE_FUNC();
  assert(a->GetOrientation() == FaceOrientation::horizontal);
  assert(b->GetOrientation() == FaceOrientation::horizontal);
  assert(a->GetPoints().size() == 3);
  assert(b->GetPoints().size() == 3);

  //Collect the points the candidates must be a subset of
  std::vector<boost::shared_ptr<Point>> points {
    a->GetPoints()
  };
  for (auto p: b->GetPoints()) { points.push_back(p); }

  std::sort(points.begin(),points.end());
  assert(std::unique(points.begin(),points.end()) == points.end());
  assert(std::count(points.begin(),points.end(),nullptr) == 0);

  //Collect the candidates
  std::vector<boost::weak_ptr<Face>> weak_candidates;
  for (auto p: a->GetPoints()) { for (auto q: p->GetConnected()) { weak_candidates.push_back(q); } }
  for (auto p: b->GetPoints()) { for (auto q: p->GetConnected()) { weak_candidates.push_back(q); } }

  std::vector<boost::shared_ptr<Face>> candidates;
  for (auto p: weak_candidates) { const auto q = p.lock(); if (q) candidates.push_back(q); }
  std::sort(candidates.begin(),candidates.end());
  candidates.erase(std::unique(candidates.begin(),candidates.end()),candidates.end());
  assert(std::count(candidates.begin(),candidates.end(),nullptr) == 0);

  //Collect the faces between
  std::vector<boost::shared_ptr<Face>> faces;
  for (auto c: candidates)
  {
    if (IsSubset(c->GetPoints(),points)) { faces.push_back(c); }
  }
  assert(std::is_sorted(faces.begin(),faces.end()));
  assert(std::unique(faces.begin(),faces.end()) == faces.end());
  assert(std::count(faces.begin(),faces.end(),nullptr) == 0);

  //Remove the faces a and b
  assert(std::count(faces.begin(),faces.end(),a) == 1);
  assert(std::count(faces.begin(),faces.end(),b) == 1);
  std::remove(faces.begin(),faces.end(),a);
  faces.pop_back();
  std::remove(faces.begin(),faces.end(),b);
  faces.pop_back();
  return faces;
}

bool ribi::trim::CellsCreator::IsSubset(
  std::vector<boost::shared_ptr<Point>> v,
  std::vector<boost::shared_ptr<Point>> w
) noexcept
{
  PROFILE_FUNC();
  std::sort(v.begin(),v.end());
  std::sort(w.begin(),w.end());
  assert(std::is_sorted(v.begin(),v.end()));
  assert(std::is_sorted(w.begin(),w.end()));
  assert(std::unique(v.begin(),v.end()) == v.end());
  assert(std::unique(w.begin(),w.end()) == w.end());
  assert(std::count(v.begin(),v.end(),nullptr) == 0);
  assert(std::count(w.begin(),w.end(),nullptr) == 0);
  std::vector<boost::shared_ptr<Point>> x;
  std::set_intersection(v.begin(),v.end(),w.begin(),w.end(),
    std::back_inserter(x));
  assert(std::count(x.begin(),x.end(),nullptr) == 0);

  return x.size() == std::min(v.size(),w.size());
}

std::vector<boost::shared_ptr<ribi::trim::Cell>> ribi::trim::CellsCreator::GetCells() noexcept
{
  return m_cells;
}

#ifndef NDEBUG
void ribi::trim::CellsCreator::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::trim::CellsCreator::Test");
  TRACE("Trying out to build cells from the testing templates")
  for (CreateVerticalFacesStrategy strategy: CreateVerticalFacesStrategies().GetAll())
  {
    for (int i=0; i!=4; ++i)
    {
      const boost::shared_ptr<Template> my_template {
        Template::CreateTest(i)
      };

      const int n_layers = 3;
      const boost::shared_ptr<CellsCreator> cells_creator {
        CellsCreatorFactory().Create(
          my_template,
          n_layers,
          1.0 * boost::units::si::meter,
          strategy
        )
      };
      const std::vector<boost::shared_ptr<Cell>> cells { cells_creator->GetCells() };
      assert(cells.size() > 0);
    }
  }
  TRACE("Specific: check if a Face really loses its neighbour: remove a prism from a cube");
  for (CreateVerticalFacesStrategy strategy: CreateVerticalFacesStrategies().GetAll())
  {
    //Create a 2x1 cell block
    const boost::shared_ptr<Template> my_template {
      Template::CreateTest(1)
    };
    assert(my_template->CountFaces() == 2);
    const int n_layers = 2;
    const boost::shared_ptr<CellsCreator> cells_creator {
      CellsCreatorFactory().Create(
        my_template,
        n_layers,
        1.0 * boost::units::si::meter,
        strategy
      )
    };
    const std::vector<boost::shared_ptr<Cell>> cells { cells_creator->GetCells() };
    assert(cells.size() == 2);
    const std::vector<boost::shared_ptr<Face>> faces_1 { cells[0]->GetFaces() };
    const std::vector<boost::shared_ptr<Face>> faces_2 { cells[1]->GetFaces() };
    //Find the one/two Faces that have a neighbour
    {
      const int n_faces_with_neighbour {
        std::count_if(faces_1.begin(),faces_1.end(),
          [](const boost::shared_ptr<Face> face)
          {
            return face->GetNeighbour().get();
          }
        )
      };
      TRACE(cells[0]->GetFaces().size());
      TRACE(cells[1]->GetFaces().size());
      TRACE(CreateVerticalFacesStrategies().ToStr(strategy));
      TRACE(n_faces_with_neighbour);
      assert(
           (strategy == CreateVerticalFacesStrategy::one_face_per_square
             && n_faces_with_neighbour == 1)
        || (strategy == CreateVerticalFacesStrategy::two_faces_per_square
             && n_faces_with_neighbour == 2)
      );
    }
    {
      const int n_faces_with_neighbour {
        std::count_if(faces_2.begin(),faces_2.end(),
          [](const boost::shared_ptr<Face> face)
          {
            return face->GetNeighbour().get();
          }
        )
      };
      assert(
           (strategy == CreateVerticalFacesStrategy::one_face_per_square
             && n_faces_with_neighbour == 1)
        || (strategy == CreateVerticalFacesStrategy::two_faces_per_square
             && n_faces_with_neighbour == 2)
      );
    }
    std::set<boost::shared_ptr<Face>> internal_faces_1;
    std::copy_if(faces_1.begin(),faces_1.end(),std::inserter(internal_faces_1,internal_faces_1.begin()),
      [](const boost::shared_ptr<Face> face)
      {
        return face->GetNeighbour().get();
      }
    );
    std::set<boost::shared_ptr<Face>> internal_faces_2;
    std::copy_if(faces_2.begin(),faces_2.end(),std::inserter(internal_faces_2,internal_faces_2.begin()),
      [](const boost::shared_ptr<Face> face)
      {
        return face->GetNeighbour().get();
      }
    );
    assert(internal_faces_1 == internal_faces_2);
  }
  TRACE("Finished ribi::trim::CellsCreator::Test successfully");
}
#endif
