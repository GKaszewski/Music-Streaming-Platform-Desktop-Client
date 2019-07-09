#pragma once

#include "Song.h"

#include "json.hpp"

using json = nlohmann::json;

class Utilities{
public:
	Utilities();
	~Utilities();
	static Song createFromJSON(json data);
	static QString encodeUrl(QString stringToEncode);
};
