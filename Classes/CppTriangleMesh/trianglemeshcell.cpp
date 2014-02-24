#include "trianglemeshcell.h"

#include <iostream>

#include "Shiny.h"

#include "trianglemeshcellfactory.h"
#include "trianglemeshface.h"
#include "trianglemeshhelper.h"
#include "trianglemeshpoint.h"
#include "trace.h"
#include "xml.h"

ribi::trim::Cell::Cell(
  const std::vector<boost::shared_ptr<Face>>& faces,
  const int index,
  const CellFactory&)
  : m_faces(faces),
    m_index{index}
{
  #ifndef NDEBUG
  Test();
  #endif
  assert(faces.size() == 8);
}

const ribi::Coordinat3D ribi::trim::Cell::CalculateCenter() const noexcept
{
  PROFILE_FUNC();
  ribi::Coordinat3D center;
  int cnt = 0;
  for(boost::shared_ptr<const Face> face: m_faces)
  {

    for(auto point: face->GetPoints())
    {
      const ribi::Coordinat3D coordinat(
        point->GetCoordinat()->GetX(),
        point->GetCoordinat()->GetY(),
        point->GetZ().value()
      );
      center += coordinat;
      ++cnt;
    }
  }
  center /= static_cast<double>(cnt);
  return center;
}


const std::vector<boost::shared_ptr<const ribi::trim::Face>> ribi::trim::Cell::GetFaces() const noexcept
{
  return AddConst(m_faces);
}

#ifndef NDEBUG
void ribi::trim::Cell::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::trim::Cell::Test");
  //Do not use the Cell its contructor! Use CellFactory::Create instead!

  //Test that in a prism-shaped Cell, all Faces are owned, and no faces have a neighbour
  {
    const boost::shared_ptr<Cell> prism {
      CellFactory().CreateTestPrism()
    };
    assert(prism);
    assert(prism->GetFaces().size() == 8);
    const std::vector<boost::shared_ptr<Face>> faces {
      prism->GetFaces()
    };
    assert(
      std::count_if(faces.begin(),faces.end(),
        [](const boost::shared_ptr<Face> face)
        {
          assert(face);
          assert(face->GetOwner());
          return face->GetNeighbour().get();
        }
      ) == 0
    );
  }
  //Test that in a prism-shaped Cell, all Faces are owned, and no faces have a neighbour
  {
    const std::vector<boost::shared_ptr<Cell>> cube {
      CellFactory().CreateTestCube()
    };
    assert(cube.size() == 2 && "A cube consists out of two prisms");
    assert(cube[0]->GetFaces().size() == 8 && "A prism consists out of 8 faces");
    assert(cube[1]->GetFaces().size() == 8 && "A prism consists out of 8 faces");
    //Concatenate the faces
    std::vector<boost::shared_ptr<Face>> faces {
      cube[0]->GetFaces()
    };
    const std::vector<boost::shared_ptr<Face>> other_faces { cube[1]->GetFaces() };
    std::copy(std::begin(other_faces),std::end(other_faces),
      std::back_inserter(faces)
    );
    std::sort(faces.begin(),faces.end());
    assert(faces.size() == 16);
    /*
    for (auto face: faces)
    {
      std::stringstream all_face_str;
      if (face)
      {
        all_face_str << face->GetIndex() << " -> " << (face->GetNeighbour() ? face->GetNeighbour()->GetIndex() : -1);
      }
      else
      {
        all_face_str << "No face";
      }
      TRACE(all_face_str.str());
    }
    */
    assert(std::count(std::begin(faces),std::end(faces),nullptr) == 0);
    assert(std::unique(faces.begin(),faces.end()) != faces.end() && "Two faces must be present twice");
    faces.erase(std::unique(std::begin(faces),std::end(faces)),faces.end());
    faces.erase(std::remove(std::begin(faces),std::end(faces),nullptr),faces.end()); //OBLIGATORY! std::unique creates nullptrs!
    /*
    TRACE(faces.size());
    TRACE(std::count(std::begin(faces),std::end(faces),nullptr));
    for (auto face: faces)
    {
      std::stringstream face_str;
      if (face)
      {
        face_str << face->GetIndex() << " -> " << (face->GetNeighbour() ? face->GetNeighbour()->GetIndex() : -1);
      }
      else
      {
        face_str << "No face";
      }
      TRACE(face_str.str());
    }
    */
    assert(faces.size() == 14 && "Two faces were in both Cells, and are now present only once");
    assert(
      std::count_if(faces.begin(),faces.end(),
        [](const boost::shared_ptr<Face> face)
        {
          assert(face);
          assert(face->GetOwner());
          return face->GetNeighbour().get();
        }
      ) == 2
    );
  }
  TRACE("Finished ribi::trim::Cell::Test successfully");
}
#endif

bool ribi::trim::operator==(const ribi::trim::Cell& lhs, const ribi::trim::Cell& rhs)
{
  return lhs.GetFaces() == rhs.GetFaces();
}

bool ribi::trim::operator!=(const ribi::trim::Cell& lhs, const ribi::trim::Cell& rhs)
{
  return !(lhs == rhs);
}

std::ostream& ribi::trim::operator<<(std::ostream& os, const ribi::trim::Cell& cell)
{
  const auto faces = cell.GetFaces();
  std::stringstream s;
  s
    << ribi::xml::ToXml("cell_index",cell.GetIndex())
  ;
  {
    std::stringstream t;
    const int n_faces { static_cast<int>(faces.size()) };
    for (int i=0; i!=n_faces; ++i)
    {
      t << ribi::xml::ToXml("face" + boost::lexical_cast<std::string>(i),faces[i]->GetIndex());
    }
    s << ribi::xml::ToXml("faces",t.str());
  }
  const std::vector<std::string> v { ribi::xml::XmlToPretty(s.str()) };
  std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"));

  return os;
}