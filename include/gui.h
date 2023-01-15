//
// Created by bertoc2 on 1/14/2023.
//

#ifndef INC_1_GUI_H
#define INC_1_GUI_H
#include <string>

int init_gui(std::string video_id, uint64_t frame_count, uint16_t framerate);
void handle_client_action(char buffer[4096]);
void handle_server_data(char buffer[4096]);

#endif // INC_1_GUI_H
