#include <string>
#include <vector>
#include "WorldWindow.h"
#include "WorldView.h"
#include "announcements.h"

extern WorldWindow * window;
extern WorldView * world_view;
vector<Announcement> msgs;

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
		if(!window->is_paused) (*iter).ttl--;

		world_view->renderText((*iter).location.x, (*iter).location.y, 0.0, (*iter).msg.c_str());
	}
	glDepthFunc(GL_LESS);
}

