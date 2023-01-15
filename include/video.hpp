#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <string>
#include <ftxui/component/component.hpp>
#include "ftxui/component/component_base.hpp"   
#include "thumbnail.h"

using namespace ftxui;

class Comment {
public:
	Comment(const std::string& author, const std::string& content) :
		author(author), content(content) {};

	~Comment() {};

	ftxui::Element create() {
		return window(text(author), text(content)) | center;
	}

	std::string author, content;
};

class Video {
public:
	Video(const std::string& title, const std::string& channelTitle, const std::string& id) :
		title(title), channelTitle(channelTitle), id(id) {};
	~Video() {};

	ftxui::Element create(bool selected = false) const {

		std::string path = "py/thumbnails/most_popular/" + id + ".jpg";

		if (selected) {
			return window(text(title) | hcenter | bold | inverted,
				Thumbnail(path)->Render()) |
				size(WIDTH, EQUAL, 62);
		} else {
		return window(text(title) | hcenter | bold,
						Thumbnail(path)->Render()) |
			size(WIDTH, EQUAL, 62);
		}
	}

	std::string title, channelTitle, id;
	std::vector<Comment> comments;
};

#endif // _VIDEO_H_
