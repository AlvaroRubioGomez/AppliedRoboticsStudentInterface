#include "student_image_elab_interface.hpp"
#include "student_planning_interface.hpp"

#include <vector>
/************Input variables generation for Unit Testing**********************/
Point add_points(Point pt1,Point pt2){
  Point result;
  result.x = pt1.x + pt2.x;
  result.y = pt1.y + pt2.y;

  return result;
}

Point sub_points(Point pt1,Point pt2){
  Point result;
  result.x = pt1.x - pt2.x;
  result.y = pt1.y - pt2.y;

  return result;
}

/*Generate graph example*/
std::vector<std::pair<Point, std::vector<Point> >> generate_graph_test(){
  std::vector<std::pair<Point, std::vector<Point> >> prm_graph_test;
  std::pair<Point, std::vector<Point>> adj_list;
  std::vector<Point> edges_list;
  Point next_edge;
  //std::pair<Point,Point > next_point;
  Point start_point = Point(0.1,0.1);
  int rows = 9; 
  int cols = 14; 
  Point x_step = Point(0.1,0);
  Point y_step = Point(0,0.1);
  
  //Initilize first point
  Point actual_point;
  for(int r=0;r<rows;r++){  //loop through rows
    actual_point.y = start_point.y + r*y_step.y; //go up 1 row     
    for(int c=0;c<cols;c++){
      edges_list.clear(); //reset edges vector
      // Next point
      actual_point.x = start_point.x + c*x_step.x; //go right 1 col     
      //std::cout << "Actual point: " << actual_point.x << ", " << actual_point.y << std::endl;
      //Check left point
      if(actual_point.x - x_step.x >= start_point.x){      
        next_edge = sub_points(actual_point,x_step);
        edges_list.push_back(next_edge);
        //std::cout << "------------ left: " << next_edge.x << ", " << next_edge.y << std::endl;        
      }
      //Check up point
      if(actual_point.y + y_step.y <= start_point.y + y_step.y*(rows - 1)){
        next_edge = add_points(actual_point,y_step);
        edges_list.push_back(next_edge);
        //std::cout << "------------ up: " << next_edge.x << ", " << next_edge.y << std::endl;  
      }
      //Check right point
      if(actual_point.x + x_step.x <= start_point.x + x_step.x*(cols - 1)){
        next_edge = add_points(actual_point,x_step);
        edges_list.push_back(next_edge);
        //std::cout << "------------ right: " << next_edge.x << ", " << next_edge.y << std::endl;  
      }
      //Check down point
      if(actual_point.y - y_step.y >= start_point.y){
        next_edge = sub_points(actual_point,y_step);
        edges_list.push_back(next_edge);   
        //std::cout << "------------ down: " << next_edge.x << ", " << next_edge.y << std::endl;       
      }
    //Add all vertexes
      adj_list = std::make_pair(actual_point,edges_list);
      // std::cout << "prm_graph element " << c + r*cols << ": " << std::endl;
      // Point V = adj_list.first;
      // std::cout << "V: " <<  V.x << ", " << V.y << "; E: ";
      // std::vector<Point> E_list = adj_list.second;
      // for (int i=0;i<E_list.size();i++){
      //    std::cout << "(" <<  E_list[i].x << ", " << E_list[i].y << "), ";
      // }      
      // std::cout << std::endl;
      prm_graph_test.push_back(adj_list);
    }   
  }
  std::cout << "graph size: " << prm_graph_test.size() << std::endl;

  return prm_graph_test;  
}

// @Ambike
/*Generate points example (from graph)*/
std::vector<Point> generate_free_space_points_test (std::vector<std::pair<Point, 
                                                      std::vector<Point> >> prm_graph_test){
  std::vector<Point> result;
  for(int i=0;i<prm_graph_test.size();i++){
    result.push_back(prm_graph_test[i].first); //first element of pair --> V
  }

  return result;
}




/* **********************Gkiri PRM space Unit Testing*************************/
void UT_sample_generation(std::vector<Polygon> obstacle_list ,img_map_def *map_param)
{
    PRM obj(obstacle_list);
    obj.generate_random_points(1.5,1.0, 100);
    std::vector<Point> free_space_points = obj.get_free_space_points();
    std::cout << " $$$$$$$$$$$$$$$$$$$$  no:of point= " << free_space_points.size() <<  std::endl;
    for (size_t i = 0; i<obstacle_list.size(); i++){
         draw_polygon(obstacle_list[i], *map_param);
    }
    for (int z=0;z<free_space_points.size();z++){
        draw_point(free_space_points[z], *map_param); 
           
    }

}

void UT_sampling_motion_plan(std::vector<Polygon> obstacle_list ,img_map_def *map_param){
    /*Gkiri  PRM sampling motion planning debugging*/
    //draw_motion_planning(obstacle_list,&map_param);
    //draw_motion_planning(inflated_obstacle_list,&map_param);

}

// @Ambike
void UT_local_planner(std::vector<Polygon> obstacle_list, img_map_def *map_param){
  // Set variables for your unit test
  PRM obj(obstacle_list);
  std::vector<std::pair<Point, std::vector<Point> >> prm_graph_test; //Output  
  prm_graph_test = generate_graph_test();
  std::vector<Point> free_space_points_test = generate_free_space_points_test(prm_graph_test);
  
  //Call your implementation on PRM.cpp    
  obj.local_planner(); //Implement your local planner in PRM.cpp

  //Retrieve the output of your function
  std::vector<std::pair<Point, std::vector<Point> >> prm_graph = obj.get_prm_graph();

  //****************************************************************************
  //********Drawing and printing the result of your local planner*************** 
  //draw polygons
  for (size_t i = 0; i<obstacle_list.size(); i++){
    draw_polygon(obstacle_list[i], *map_param);
  }
  //draw graph
  std::pair<Point, std::vector<Point>> graph_node;
  Point V;
  std::vector<Point> E; 
  arc_extract edge_line;
  Point E_point;
  for(int i=0; i<prm_graph_test.size(); i++){
    //std::cout << "prm raph size: " << prm_graph_test.size() << std::endl;
    graph_node = prm_graph_test[i];
    V = graph_node.first; //Vertex
    std::cout << "prm V: " << V.x << ", " << V.y << std::endl;
    E = graph_node.second; //Edges
    //Draw edges    
    for(int j=0;j<E.size();j++){ 
      std::cout << "Edge: " << E[j].x << ", " << E[j].y << std::endl;
      edge_line = to_arc_extract_type(V,E[j],true);
      draw_line(edge_line, *map_param);
    }
    //Draw vertex
    draw_point(V, *map_param, cv::Scalar(255,0,0));
  }

}

void UT_global_planner(std::vector<Polygon> obstacle_list, img_map_def *map_param){
  //Set variables for unit test
  PRM obj(obstacle_list);
  Point start = Point(0.1,0.1);
  Point goal = Point(1.3,0.8);
  //Point goal = Point(0.3,0.1);
  std::vector<std::pair<Point, std::vector<Point> >> prm_graph_test;  
  prm_graph_test = generate_graph_test();
  obj.set_prm_graph(prm_graph_test);  //set prm_graph with the test one
  prm_graph_test = obj.get_prm_graph(); //retrieve to check it was saved correctly

  //****************************************************************************
  //********Drawing and printing the initial prm graph and star/goal************
  //draw graph
  std::pair<Point, std::vector<Point>> graph_node;
  Point V;
  std::vector<Point> E; 
  arc_extract edge_line;
  Point E_point;
  for(int i=0; i<prm_graph_test.size(); i++){
    //std::cout << "prm raph size: " << prm_graph_test.size() << std::endl;
    graph_node = prm_graph_test[i];
    V = graph_node.first; //Vertex
    std::cout << "prm V: " << V.x << ", " << V.y << std::endl;
    E = graph_node.second; //Edges
    //Draw edges    
    for(int j=0;j<E.size();j++){ 
      std::cout << "Edge: " << E[j].x << ", " << E[j].y << std::endl;
      edge_line = to_arc_extract_type(V,E[j],true);
      draw_line(edge_line, *map_param);
    }
    //Draw vertex
    draw_point(V, *map_param, cv::Scalar(255,0,0)); 
  }
  //Draw start and goal
  draw_point(start, *map_param, cv::Scalar(0,255,0));
  draw_point(goal, *map_param, cv::Scalar(0,255,0));  

  //****************************************************************************
  //********Global planner******************************************************    
  obj.global_planner(start,goal);
  std::vector<Point> global_planner_path = obj.get_global_planner_path(); 

  // //****************************************************************************
  // //********Drawing and printing the global planner path************************
  for(int i=0;i<global_planner_path.size();i++){
    //draw_point(global_planner_path[i], *map_param, cv::Scalar(0,255,0));
    //Draw path
    if(i<global_planner_path.size()-1){         
      edge_line = to_arc_extract_type(global_planner_path[i],global_planner_path[i+1],true);
      draw_line(edge_line, *map_param, cv::Scalar(0,255,0));    
    }
    std::cout << "gpp "<< i << ": " << global_planner_path[i].x << ", " << global_planner_path[i].y << std::endl;
  }
  
}


/*----------------------------- Dubins section--------------------------------*/
void UT_dubins_curve_test(struct arc_extract *three_seg,img_map_def *map_param)
{
    //Visualize dubins curve test
    arc_extract dubins_line;

    for(int i=0;i<3;i++){
      switch (three_seg[i].LSR)
      {
      case 0: // Left arc
        std::cout << "Left arc" << std::endl;
        //Print values
        std::cout << "Start point: " << three_seg[i].start_point.x 
                                        << ", " << three_seg[i].start_point.y << std::endl;
        std::cout << "End point: " << three_seg[i].end_point.x 
                                        << ", " << three_seg[i].end_point.y << std::endl;
        std::cout << "Radius: " << three_seg[i].radius << std::endl;
        std::cout << "Center: " << three_seg[i].center.x 
                          << ", " << three_seg[i].center.y <<std::endl;
        std::cout << "Length: " << three_seg[i].length <<std::endl;
        std::cout << "LSR: " << three_seg[i].LSR <<std::endl;

        draw_dubins_segment(three_seg[i], *map_param);
                             
        
        break;
      case 1: // Straight line
        std::cout << "Straight line" << std::endl;
        dubins_line = to_arc_extract_type(three_seg[i].start_point,
                                                three_seg[i].end_point,true);
        //Print values
        std::cout << "Start point: " << three_seg[i].start_point.x 
                                        << ", " << three_seg[i].start_point.y << std::endl;
        std::cout << "End point: " << three_seg[i].end_point.x 
                                        << ", " << three_seg[i].end_point.y << std::endl;
        std::cout << "Radius: " << three_seg[i].radius << std::endl;
        std::cout << "Center: " << three_seg[i].center.x 
                          << ", " << three_seg[i].center.y << std::endl;
        std::cout << "Length: " << three_seg[i].length << std::endl;
        std::cout << "LSR: " << three_seg[i].LSR <<std::endl;
        std::cout << "Calculated Length: " << dubins_line.length << std::endl;

        draw_dubins_segment(dubins_line,*map_param);
        break;
      case 2: // Right arc
        std::cout << "Right arc" << std::endl;
        //Print values
        std::cout << "Start point: " << three_seg[i].start_point.x 
                                        << ", " << three_seg[i].start_point.y << std::endl;
        std::cout << "End point: " << three_seg[i].end_point.x 
                                        << ", " << three_seg[i].end_point.y << std::endl;
        std::cout << "Radius: " << three_seg[i].radius << std::endl;
        std::cout << "Center: " << three_seg[i].center.x 
                          << ", " << three_seg[i].center.y << std::endl;
        std::cout << "Length: " << three_seg[i].length <<std::endl;
        std::cout << "LSR: " << three_seg[i].LSR <<std::endl;      

        draw_dubins_segment(three_seg[i], *map_param);
        break;
      
      default:
        std::cout << "Unknown LSR" << std::endl;
        break;
      }
    }

}


void UT_arc_draw_test(img_map_def *map_param)
{
    //dubin drawing test
    arc_extract dt[2];
    //line
    dt[0].start_point = Point (0.456287, 0.599802);
    dt[0].end_point = Point (1.24375, 0.550198);
    dt[0].LSR = 1;

    //arc
    dt[1].start_point = Point (1.24375, 0.550198);
    dt[1].end_point = Point (1.25, 0.75);
    dt[1].radius = 0.1;
    dt[1].center = Point(1.34683, 0.646974);
    dt[1].LSR = 0;

    std::cout << "x pre: " <<  dt[1].center.x << std::endl;
    draw_dubins_segment(dt[0],*map_param);
    draw_dubins_segment(dt[1],*map_param);
    draw_point(dt[1].center,*map_param);
    std::cout << "x pos " << dt[1].center.x << std::endl;

    arc_param curve_angles = calculate_arc_drawing_angles(dt[1]);
    std::cout << "Curve Rotation angle: " << curve_angles.rotation_angle << std::endl;
    std::cout << "Curve Angle btw cs & ce: " << curve_angles.angle_cs_ce << std::endl;


}