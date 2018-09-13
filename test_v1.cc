/* Written by Can Alper
 *
 * Simple c++ program to test non-visible engine behaviour.
 *
 */

#include <iostream>

#include "HiveEngine/Entity.h"
#include "HiveEngine/Utilities.h"

int main(int argc, char* argv[]){
    std::cout << "-- START --" << std::endl;

    // Creating an entity
    HiveEngine::Entity e(glm::vec3(6.0, 0.0, 0.3), 0.5, 100.0);

    // Movement test
    // e should start going left and continue, slowly...
    e.apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-0.3, 0.0, 0.0), true);

    // Rotation test
    // e should start rotating on x+ axis, slowly...
    // we use symmetrical force to prevent any other unnecessary moves
    e.apply_force(glm::vec3(0.0, 0.0, e.get_radius()), glm::vec3(0.0, 0.02, 0.0), true);
    e.apply_force(glm::vec3(0.0, 0.0, -e.get_radius()), glm::vec3(0.0, -0.02, 0.0), true);

    for (int i = 0; i < 10; ++i) {
        std::cout << "-----------------" << std::endl;
        std::cout << "ITER: " << i << std::endl;
        std::cout << "pos: " << HiveEngine::vec3_to_str(e.calculate_position()) << std::endl;
        std::cout << "rot mat: \n\t"
        << HiveEngine::vec3_to_str(e.calculate_rotation_matrix()[0]) << " \n\t"
        << HiveEngine::vec3_to_str(e.calculate_rotation_matrix()[1]) << " \n\t"
        << HiveEngine::vec3_to_str(e.calculate_rotation_matrix()[2]) << " "
        << std::endl;

        // Calculate entity for 1 iteration;
        // This function is virtual, so you can implement your own entity.
        // If you want movement behaviour make sure to call the super version.
        e.step(30);
    }

    std::cout << "--  END  --" << std::endl;
    return 0;
}
