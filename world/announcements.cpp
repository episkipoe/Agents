#include <GL/glut.h>
#include <graphics/graphics.h>
#include <string>
#include <vector>
#include "announcements.h"

extern float text_size;
vector<Announcement> msgs;
extern bool is_paused;

void add_announcement(std::string msg, Point *location, int ttl) {
	if(!location || ttl<=0) return ;
	Announcement newMsg;
	newMsg.msg = msg;
	newMsg.location = *location;
	newMsg.ttl = ttl;
	msgs.push_back(newMsg);	
}

void draw_announcements(void) {
	if(msgs.size() <= 0) return ;
	glDepthFunc(GL_ALWAYS);
	vector <Announcement>::iterator iter;
	for(iter = msgs.begin() ; iter!=msgs.end() ; iter++) {
		if((*iter).ttl<=0) {
			iter = msgs.erase(iter);
			if(iter==msgs.end()) break;
			continue;
		}
		if(!is_paused) (*iter).ttl--;

		drawText((*iter).location.x, (*iter).location.y, (*iter).msg.c_str(), text_size);
	}
	glDepthFunc(GL_LESS);
}

