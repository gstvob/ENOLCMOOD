#include "RayCasting.h"

RayCasting::RayCasting()
{}

RayCasting::~RayCasting()
{
	for (auto& [depth, texture, clip, x, y, width, height] : objectsToRender) {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}
	objectsToRender.clear();
}

void RayCasting::Update(Player& player, Map& map) {
	RayCast(player, map);
	ObjectsToRender(map);
}

void RayCasting::ObjectsToRender(Map& map) {
	objectsToRender.clear();

	for (int i = 0; i < raycastingResults.size(); i++) {
		const auto& [depth, projectionHeight, texture, offset] = raycastingResults[i];
		std::tuple<float, SDL_Texture*, SDL_Rect, float, float, float, float> wallsData;
		if (projectionHeight < HALF_HEIGHT * 2) {
			SDL_Rect texRect = { offset * (TEXTURE_SIZE - SCALE), 0, SCALE, TEXTURE_SIZE };
			float x = i * SCALE;
			float y = HALF_HEIGHT - (int)(projectionHeight / 2);
			wallsData = { depth, map.wallTexturesMap[texture], texRect, x, y, SCALE, projectionHeight};
		} else {
			float clampedHeight = TEXTURE_SIZE * HALF_HEIGHT * 2 / projectionHeight;
			SDL_Rect texRect = { offset * (TEXTURE_SIZE - SCALE), HALF_TEXTURE_SIZE - (int)(clampedHeight/2), SCALE, clampedHeight};
			float x = i * SCALE;
			float y = 0;
			wallsData = { depth, map.wallTexturesMap[texture], texRect, x, y, SCALE, HALF_HEIGHT * 2 };
		}
		objectsToRender.push_back(wallsData);
	}
}


void RayCasting::RayCast(Player& player, Map& map)
{
	raycastingResults.clear();
	const auto [ox, oy] = player.GetPosition();
	const auto [xMap, yMap] = player.GetPositionInMap();

	int texHor = 1, texVert = 1;
	float rayAngle = player.GetAngle() - HALF_FOV + 0.0001;

	for (auto ray = 0; ray < NUM_RAYS; ray++) {
		float sinA = sin(rayAngle);
		float cosA = cos(rayAngle);

		float yHor = sinA > 0 ? yMap + 1 : yMap - 1e-6;
		float dy = sinA > 0 ? 1 : -1;

		float depthHor = (yHor - oy) / sinA;
		float xHor = ox + depthHor * cosA;

		float deltaDepth = dy / sinA;
		float dx = deltaDepth * cosA;

		for (auto i = 0; i < MAX_DEPTH; i++) {
			if (map.mapPositions.find({ (int)xHor, (int)yHor }) != map.mapPositions.end()) {
				texHor = map.mapPositions[{(int)xHor, (int)yHor}];
				break;
			}
			xHor += dx;
			yHor += dy;
			depthHor += deltaDepth;
		}


		float xVert = cosA > 0 ? xMap + 1 : xMap - 1e-6;
		dx = cosA > 0 ? 1 : -1;

		float depthVert = (xVert - ox) / cosA;
		float yVert = oy + sinA * depthVert;

		deltaDepth = dx / cosA;
		dy = deltaDepth * sinA;

		for (auto i = 0; i < MAX_DEPTH; i++) {
			if (map.mapPositions.find({ (int)xVert, (int)yVert }) != map.mapPositions.end()) {
				texVert = map.mapPositions[{(int)xVert, (int)yVert}];
				break;
			}
			xVert += dx;
			yVert += dy;
			depthVert += deltaDepth;
		}

		float depth = 0.0f;
		float textureOffset = 0;
		int texture;
		if (depthVert < depthHor) {
			depth = depthVert;
			texture = texVert;
			yVert = fmod(yVert, 1);
			if (yVert < 0) {
				yVert += 1;
			}
			textureOffset = cosA > 0 ? yVert : 1 - yVert;
		} else {
			depth = depthHor;
			texture = texHor;
			xHor = fmod(xHor, 1);
			if (xHor < 0) {
				xHor += 1;
			}
			textureOffset = sinA > 0 ? 1 - xHor : xHor;
		}
	

		depth = depth *  cos(player.GetAngle() - rayAngle);
		float projectionHeight = SCREEN_DIST / (depth + 0.0001);
		
		raycastingResults.push_back({ depth, projectionHeight, texture, textureOffset });

		rayAngle += DELTA_ANGLE;
	}
}

