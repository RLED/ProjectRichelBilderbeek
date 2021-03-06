#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "pvdbfilefactory.h"

#include <cassert>
#include "pvdbconceptmapfactory.h"
#include "pvdbclusterfactory.h"
#include "pvdbconceptmap.h"
#include "pvdbcluster.h"
#include "pvdbclusterfactory.h"
#include "pvdbfile.h"
#include "trace.h"

#ifdef PVDB_KEEP_NAMESPACE_IN_CPP_FILES
namespace pvdb {
#endif

const boost::shared_ptr<pvdb::File> pvdb::FileFactory::Create()
{
  boost::shared_ptr<pvdb::File> file(new pvdb::File);
  assert(file);
  return file;
}

const boost::shared_ptr<pvdb::File> pvdb::FileFactory::DeepCopy(const boost::shared_ptr<const pvdb::File>& file)
{
  assert(file);
  assert(file->GetCluster());
  const boost::shared_ptr<pvdb::Cluster> cluster = pvdb::ClusterFactory::DeepCopy(file->GetCluster());
  assert(cluster);
  assert(file->GetCluster());
  assert(IsEqual(*cluster,*file->GetCluster()));

  assert(file->GetConceptMap());
  const boost::shared_ptr<pvdb::ConceptMap> concept_map = pvdb::ConceptMapFactory::DeepCopy(file->GetConceptMap());
  assert(concept_map);
  assert(IsEqual(*concept_map,*file->GetConceptMap()));

  const boost::shared_ptr<pvdb::File> p(
    new File(
      file->GetAbout(),
      file->GetAssessorName(),
      cluster,
      concept_map,
      file->GetStudentName(),
      file->GetVersion()));
  assert(p);
  #ifndef NDEBUG
  if (file != p)
  {
    TRACE(pvdb::File::ToXml(*file));
    TRACE(pvdb::File::ToXml(*p));
  }
  #endif
  assert(file == p);
  assert(IsEqual(*file,*p));
  return p;
}

const std::vector<boost::shared_ptr<pvdb::File> > pvdb::FileFactory::GetTests()
{
  std::vector<boost::shared_ptr<pvdb::File> > v;
  //[0]: empty file
  {
    v.push_back(Create());
  }
  //[1]: file with focal question
  {
    boost::shared_ptr<pvdb::File> f = Create();
    assert(f);
    const boost::shared_ptr<pvdb::ConceptMap> concept_map
      = ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(0);
    assert(concept_map);
    f->SetConceptMap(concept_map);
    v.push_back(f);
  }
  //[2]: file with complex concept map
  {
    boost::shared_ptr<pvdb::File> f = Create();
    assert(f);
    const boost::shared_ptr<pvdb::ConceptMap> concept_map
      = ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(15);
    assert(concept_map);
    f->SetConceptMap(concept_map);
    v.push_back(f);
  }
  //[3]: file with complex concept map and complex cluster
  {
    boost::shared_ptr<pvdb::File> f = Create();
    assert(f);
    f->SetStudentName("pvdb::FileFactory::GetTests()[3] name");
    const boost::shared_ptr<pvdb::ConceptMap> concept_map
      = ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(15);
    assert(concept_map);
    f->SetConceptMap(concept_map);
    const boost::shared_ptr<pvdb::Cluster> cluster
      = ClusterFactory::GetTests().at(3);
    assert(cluster);
    f->SetCluster(cluster);
    v.push_back(f);
  }
  //[4]: file with rated complex concept map and complex cluster
  {
    boost::shared_ptr<pvdb::File> f = Create();
    assert(f);
    f->SetStudentName("pvdb::FileFactory::GetTests()[4] name");
    const boost::shared_ptr<pvdb::ConceptMap> concept_map
      = ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(16);
    assert(concept_map);
    f->SetConceptMap(concept_map);
    const boost::shared_ptr<pvdb::Cluster> cluster
      = ClusterFactory::GetTests().at(3);
    assert(cluster);
    f->SetCluster(cluster);
    v.push_back(f);
  }

  assert(std::count_if(v.begin(),v.end(),[](const boost::shared_ptr<pvdb::File>& p) { return !p; } ) == 0);
  //assert(std::all_of(v.begin(),v.end(),[](const boost::shared_ptr<pvdb::File>& p) { return p; } ));
  return v;
}



#ifdef PVDB_KEEP_NAMESPACE_IN_CPP_FILES
} //~namespace pvdb
#endif
