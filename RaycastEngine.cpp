// Lodev you're a legend
// ^ This guy has a super cool name, Lode Vandevenne. 
// He's also the person who's guide I'm follwoing for the most part.

#include "RaycastEngine.h"

RaycastEngine::RaycastEngine(int w, int h) : screenWidth(w), screenHeight(h), isRunning(true), renderDist(100) {}

void RaycastEngine::addEntity(Entity* entity) { entities.push_back(entity); } 

void RaycastEngine::render(sf::RenderWindow& window, const Player& player, const Map& map) {
    // Z-buffer stores wall distances for each screen column (for sprite occlusion)
    // Oclusion is when one object hides another.
    // This makes sure not to display any sprites that shouldn't be displayed
    std::vector<float> zBuffer(screenWidth, 0.0f);
    
    // === WALL RAYCASTING ===
    // Cast one ray per screen column/width (vertical strip for each column)
    for (int x = 0; x < screenWidth; ++x) {
        // Calculate ray direction for current screen column
        // cameraX ranges from -1 (left) to +1 (right)
        float cameraX = 2 * x / static_cast<float>(screenWidth) - 1;
        sf::Vector2f rayDir(player.dir.x + player.plane.x * cameraX, player.dir.y + player.plane.y * cameraX);

        // Current map position (which grid square we're in)
        sf::Vector2i mapPos(static_cast<int>(player.position.x), static_cast<int>(player.position.y));
        
        // Distance the ray travels to cross one grid square in x or y direction
        sf::Vector2f deltaDist(std::fabs(1 / rayDir.x), std::fabs(1 / rayDir.y));

        sf::Vector2f sideDist;  // Distance from current position to next grid line
        sf::Vector2i step;      // Direction to step in map grid (+1 or -1)
        int side;               // Which side was hit: 0 for x-side, 1 for y-side

        // Calculate step direction and initial sideDist
        if (rayDir.x < 0) { 
            step.x = -1; 
            sideDist.x = (player.position.x - mapPos.x) * deltaDist.x; 
        } else { 
            step.x = 1; 
            sideDist.x = (mapPos.x + 1 - player.position.x) * deltaDist.x; 
        }

        if (rayDir.y < 0) { 
            step.y = -1; 
            sideDist.y = (player.position.y - mapPos.y) * deltaDist.y; 
        } else { 
            step.y = 1; 
            sideDist.y = (mapPos.y + 1 - player.position.y) * deltaDist.y; 
        }

        // DDA (Digital Differential Analysis) algorithm - step through grid until we hit a wall
        bool hit = false;
        while (!hit) {
            // Jump to next grid square (either in x or y direction)
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                side = 0;
            } else {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                side = 1;
            }
            // Check if we hit a wall (grid value > 0)
            if (map.grid[mapPos.y][mapPos.x] > 0) hit = true;
        }

        // Calculate perpendicular distance to wall (avoids fisheye effect)
        float perpWallDist = (side == 0) ? sideDist.x - deltaDist.x : sideDist.y - deltaDist.y;

        // side note: using actual ray distance to the wall makes the wall look curved (fish eye effect)
        // since we have used planes (not rays themselves), we dont face taht issue

        // Store distance in z-buffer for sprite occlusion 
        zBuffer[x] = perpWallDist;

        // Calculate wall height on screen (inversely proportional to distance, the farther the wall the smaller it appears)
        int lineHeight = static_cast<int>(screenHeight / perpWallDist);
        
        // Calculate where to start and end drawing the wall stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // Choose wall color (darker for y-sides to create depth perception)
        sf::Color color = sf::Color(36, 179, 74);  
        if (side == 1) color = sf::Color(50, 168, 82); 

        // Draw vertical stripe for this wall slice
        sf::RectangleShape strip(sf::Vector2f(1, drawEnd - drawStart));
        strip.setPosition(static_cast<float>(x), static_cast<float>(drawStart));
        strip.setFillColor(color);
        window.draw(strip);
    }
    
    
    // === SPRITE (more like circle) CASTING ===
    // Render entities (seekers) as red circles
    std::vector<SpriteInfo> sprites;
    
    // Calculate distance from player to each entity
    for (size_t i = 0; i < entities.size(); ++i) {
        double dx = entities[i]->position.x - player.position.x;
        double dy = entities[i]->position.y - player.position.y;
        double dist = dx * dx + dy * dy;  // Squared distance, (faster, good enough for sorting)
        sprites.push_back({i, dist});
    }
    
    // Sort sprites by distance (farthest first) for proper rendering order
    std::sort(sprites.begin(), sprites.end());

    // Render each sprite
    for (const auto& sprite : sprites) {
        Entity* entity = entities[sprite.index];
        // Only render Seeker type (Enemy is default)
        if (entity->getType() != "Enemy" && entity->getType() != "Seeker") continue;

        // Sprite position relative to player
        double spriteX = entity->position.x - player.position.x;
        double spriteY = entity->position.y - player.position.y;

        // Transform sprite position to camera space using inverse camera matrix
        // A lot of big words just to say that it converts coordinates (x, y) into what the camera can actually see
        double invDet = 1.0 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);
        double transformX = invDet * (player.dir.y * spriteX - player.dir.x * spriteY);         // how far to the left.right the sprite is
        double transformY = invDet * (-player.plane.y * spriteX + player.plane.x * spriteY);    // how far away the sprite is

        // Skip sprites behind the player, just like i skip [REDACTED] classes
        if (transformY <= 0.0) continue;

        // Calculate sprites horizontal screen position
        int spriteScreenX = static_cast<int>((screenWidth / 2.0) * (1.0 + transformX / transformY));

        // Calculate sprite size on screen (closer = bigger, and bigger = better, common knowledge)
        int spriteHeight = static_cast<int>(std::abs(screenHeight / transformY) * entity->size);
        int spriteWidth = spriteHeight;  // Square bounding box for circle

        // Calculate vertical drawing bounds
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + screenHeight / 2;
        if (drawEndY >= screenHeight) drawEndY = screenHeight - 1;

        // Calculate horizontal drawing bounds
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenWidth) drawEndX = screenWidth - 1;

        // Procedural circle rendering
        const float radiusNorm = 0.5f;  // Radius in normalized [0,1] texture space (it isnt normalized right now, but will be when we calculate u)
        const sf::Color enemyColor = sf::Color::Red;

        // Draw sprite column by column
        for (int stripe = drawStartX; stripe < drawEndX; ++stripe) {
            // Skip if sprite is behind a wall at this column
            if (transformY >= zBuffer[stripe]) continue;

            // Calculate normalized horizontal texture coordinate [0,1]
            double u = static_cast<double>(stripe - (spriteScreenX - spriteWidth / 2)) / spriteWidth;

            // side note: normalizing creates a coordinate system independent of pixels themselves. suuuuuuuuuuuuuuuuper useful

            // Check if we're inside the circle horizontally
            // This is just optimization, if the stripe we are going to draw is outside the circle bounds, why waste our oh so precious computing power
            double centerU = 0.5;
            double du = u - centerU;
            if (std::fabs(du) > radiusNorm) continue;  // Outside circle

            // Calculate vertical extent of circle at this horizontal position (chord length)
            // chord = line connecting two points of a circle
            // So what we are doing is displaying each (vertical) chord.
            // Pythagorean theorem go brrrrrrrrr
            double halfChord = std::sqrt(radiusNorm * radiusNorm - du * du);
            double vStart = 0.5 - halfChord;
            double vEnd = 0.5 + halfChord;

            // side note: the formula [ sqrt(r^2 - du^2) ] actually calculates the height of the circle from the horizontal center
            // this is why its a half chord, subtracting that from vStart and adding that to vEnd gives us the full chord 

            // Convert to screen pixel coordinates
            int stripStartY = static_cast<int>(drawStartY + vStart * spriteHeight);
            if (stripStartY < drawStartY) stripStartY = drawStartY;
            int stripEndY = static_cast<int>(drawStartY + vEnd * spriteHeight);
            if (stripEndY > drawEndY) stripEndY = drawEndY;

            if (stripStartY >= stripEndY) continue;

            // Finally draw vertical stripe of the circle. 
            sf::RectangleShape strip(sf::Vector2f(1.0f, static_cast<float>(stripEndY - stripStartY)));
            strip.setPosition(static_cast<float>(stripe), static_cast<float>(stripStartY));
            strip.setFillColor(enemyColor);
            window.draw(strip);
        }
    }
}

// This is way too complicated, I should've done soemthing simpler.
// Pls help. Im under the water.