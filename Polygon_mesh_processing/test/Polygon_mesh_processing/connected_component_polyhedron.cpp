#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/boost/graph/properties_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <iostream>
#include <fstream>


namespace PMP = CGAL::Polygon_mesh_processing;

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Polyhedron_3<Kernel> Mesh;
typedef CGAL::Polyhedron_3<Kernel,CGAL::Polyhedron_items_with_id_3> Mesh_with_id;

void mesh_with_id(const char* argv1)
{
  typedef boost::graph_traits<Mesh_with_id>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<Mesh_with_id>::face_descriptor face_descriptor;

  Mesh_with_id sm;
  std::ifstream in(argv1);
  in >> sm;

  int i=0;
  BOOST_FOREACH(face_descriptor f, faces(sm)){
    f->id() = i++;
  } 
  i=0;
  BOOST_FOREACH(vertex_descriptor v, vertices(sm)){
    v->id() = i++;
  }

  std::vector<face_descriptor> cc;
  face_descriptor fd = *faces(sm).first;
  PMP::connected_component(fd,
                           sm,
                           std::back_inserter(cc));

  std::cerr << cc.size() << " faces in the CC of " << &*fd << std::endl;

  boost::vector_property_map<int,
    boost::property_map<Mesh_with_id, boost::face_index_t>::type>
      fccmap(get(boost::face_index,sm));

  std::size_t num = PMP::connected_components(sm,
                                              fccmap);
  
  std::cerr << "The graph has " << num << " connected components (face connectivity)" << std::endl;

  PMP::keep_largest_connected_components(sm,2);

 std::ofstream ofile("blobby_2cc_id.off");
 ofile << sm << std::endl;
 ofile.close();
}

void mesh_no_id(const char* argv1)
{
  typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;

  Mesh sm;
  std::ifstream in(argv1);
  in >> sm;

  
  std::vector<face_descriptor> cc;
  face_descriptor fd = *faces(sm).first;
  PMP::connected_component(fd,
                           sm,
                           std::back_inserter(cc));


  std::cerr << cc.size() << " faces in the CC of " << &*fd << std::endl;
  boost::property_map<Mesh,boost::vertex_external_index_t>::type vim 
    = get(boost::vertex_external_index,sm);

  boost::property_map<Mesh,boost::face_external_index_t>::type fim 
    = get(boost::face_external_index,sm);

  boost::vector_property_map<int,
    boost::property_map<Mesh, boost::face_external_index_t>::type>
      fccmap(fim);

  std::size_t num = PMP::connected_components(sm,
    fccmap,
    PMP::parameters::face_index_map(fim));
  
  std::cerr << "The graph has " << num << " connected components (face connectivity)" << std::endl;
  //BOOST_FOREACH(face_descriptor f , faces(sm)){
  //  std::cout  << &*f << " in connected component " << fccmap[f] << std::endl;
  //}

  PMP::keep_largest_connected_components(sm
    , 2
    , PMP::parameters::vertex_index_map(vim).
      face_index_map(fim));

  std::ofstream ofile("blobby_2cc_no_id.off");
  ofile << sm << std::endl;
  ofile.close();
}


int main(int argc, char* argv[]) 
{
  const char* filename = (argc > 1) ? argv[1] : "data/blobby_3cc.off";
  mesh_with_id(filename);
  mesh_no_id(filename);
  return 0;
}
