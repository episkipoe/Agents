#ifndef ANNOUNCEMENTS_H
#define ANNOUNCEMENTS_H

#include <common/point.h>

struct Announcement {
	std::string msg;
	Point location;
	int ttl;
};

void add_announcement (std::string msg, Point * location, int ttl);
void draw_announcements(void);

#endif
