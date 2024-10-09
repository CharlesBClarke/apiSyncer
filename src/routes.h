#ifndef ROUTES_H
#define ROUTES_H

#include "ObjectNode.h"
#include <crow.h>
#include <vector>

void setupRoutes(crow::SimpleApp &app,
                 const std::vector<std::weak_ptr<ObjectNode>> &roots);

#endif // ROUTES_H
