#pragma once

#include "student_image_elab_interface.hpp"
#include "student_planning_interface.hpp"

#include "Utils.hpp"
#include "PRM.h"

//struct
struct mission_output_0{
    Path path;
    DubinsCurve dubins_path;
};
struct mission_output_12{
    Path path;
    std::vector<arc_extract> path_final_draw;
    std::vector<Point> free_space_points;
    std::vector<std::pair<Point, std::vector<Point> >> prm_graph;
    std::vector<Point> global_planner_path;
};


/**
 * Return the final pose of the robot at the gate (keeping a safe distance to the wall
 * and with a proper angle depending on the location of the gate)
 * 
 * @param gate - polygon representing the gate location
 * @param gate_pose - pose at the gate (double gate_pose[3])
 * @param map_h - height of map (y-axis)
 * @param map_w - width of map (x-axis)
 * @output - pose at gate passed by reference
*/
void get_gate_pose(const Polygon& gate, double map_h, double map_w, double robot_length,  double* gate_pose);

/**
 * Perform mission 0: Robot goes from actual position to the gate (no obstacles considered)
 * 
 * @param dubins_param - dubins parameters k_max and discretizer_size
 * @param start_pose - starting pose of the robot
 * @param gate_pose - pose at the gate 
 * @output Path - Return path to be followed by the robot to complete the mission. 
*/
mission_output_0 mission_0(dubins_param dubins_param, double start_pose[3], double gate_pose[3]);


/**
 * Perform mission 1.5: Robot goes from actual position to the gate avoiding obstacles and collecting
 * victims depending on the cost, being cost = reward - distance to victim
 * 
 * @param PRM_param - All parameters needed for PRM (obstacle list, map dimensions, number of samples)
 * @param dubins_param - dubins parameters k_max and discretizer_size
 * @param start_pose - starting pose of the robot
 * @param gate_pose - pose at the gate 
 * @param bias_points - bias points over where the robot must pass.
 * @param delta - tune angle in degrees for dubins planner
 * @output Path - Return path to be followed by the robot to complete the mission
*/
mission_output_12 mission_1(PRM_param PRM_param, dubins_param dubins_param, double start_pose[3], double gate_pose[3], std::vector<Point> bias_points, double delta);


/**
 * Perform mission 2: Robot goes from actual position to the gate avoiding obstacles and collecting
 * victims depending on the cost, being cost = reward - distance to victim
 * 
 * @param PRM_param - All parameters needed for PRM (obstacle list, map dimensions, number of samples)
 * @param dubins_param - dubins parameters k_max and discretizer_size
 * @param start_pose - starting pose of the robot
 * @param gate_pose - pose at the gate 
 * @param victim_list - list of victims. Each victim represented by its number and a polygon
 * @param delta - tune angle in degrees for dubins planner
 * @output Path - Return path to be followed by the robot to complete the mission
*/
mission_output_12 mission_2(PRM_param PRM_param, dubins_param dubins_param, double start_pose[3], double gate_pose[3], std::vector<std::pair<int,Polygon>> victim_list, double delta);



