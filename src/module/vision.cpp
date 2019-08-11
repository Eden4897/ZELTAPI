#include "_header.h"

namespace vision{
  int getWidthBySig(std::uint32_t size_id,std::uint8_t sig_id){
    return vision_sensor.get_by_sig(size_id ,sig_id).width;
  }
  int getHeightBySig(std::uint32_t size_id,std::uint8_t sig_id){
    return vision_sensor.get_by_sig(size_id ,sig_id).height;
  }
  int getXBySig(std::uint32_t size_id,std::uint8_t sig_id){
    return vision_sensor.get_by_sig(size_id ,sig_id).x_middle_coord;
  }
  int getYBySig(std::uint32_t size_id,std::uint8_t sig_id){
    return vision_sensor.get_by_sig(size_id ,sig_id).y_middle_coord;
  }
  int getWidthByCode(uint32_t size_id,pros::vision_color_code_t color_code){
    return vision_sensor.get_by_code(size_id ,color_code).width;
  }
  int getHeightByCode(std::uint32_t size_id,pros::vision_color_code_t color_code){
    return vision_sensor.get_by_code(size_id ,color_code).height;
  }
  int getAngleByCode(std::uint32_t size_id,pros::vision_color_code_t color_code){
    return vision_sensor.get_by_code(size_id ,color_code).angle;
  }
  int getXByCode(std::uint32_t size_id,pros::vision_color_code_t color_code){
    return vision_sensor.get_by_code(size_id ,color_code).x_middle_coord;
  }
  int getYByCode(std::uint32_t size_id,pros::vision_color_code_t color_code){
    return vision_sensor.get_by_code(size_id ,color_code).y_middle_coord;
  }
}
