#pragma once
#define _USE_MATH_DEFINES
#include <vectors.h>
#include <geometry.h>
#include <math.h>
#include <GL/glew.h>


class SnakePiece : public Geometry
{
private:
	int res = 30;
	float radius; //current radius
	vec3 origo;	//current origo
	vec3 origoBeg;	//first circle's origo
	vec3 origoEnd;	//last circle's origo
	vec3 pointsBeg1;	//first circle current point
	vec3 pointsEnd1;	//last circle current point
	vec3 pointsBeg2;	//first circle  point in the next raw
	vec3 pointsEnd2;	//last circle point in the next raw
	vec3 points;		//current point
	float rotAngle2;	//angle witch rotate the points around the z axis (with offset)
	int antiClock = -1;	
	int angleOffset = 0;	//M_PI/2 * x : find the correct rotation around the z axis
	bool curved = false;
	bool reverse = false;	//reverse the normals when its needed (straight pieces)
	std::vector<vec3> centerPoints;	//circle's origos for normals
	


public:
	SnakePiece(float radBeg, float radEnd, vec2 dirBeg, vec2 dirEnd)
	{
		//straight or curved piece
		if (abs(abs(dirBeg.x) + abs(dirEnd.x) - 1) <= 0.01) {
			antiClock = 1;
			curved = true;
		}
		float rotAngle = M_PI * 2 / res;	//angle witch rotate the points around the current x-y axis
		float uvStep = 1.0f / res;
		float avrRad = (radBeg + radEnd) / 2;	//avrages out the first and last circle's radian

		if (curved)
		{
			//carculate the offset if it's curved
			if (dirBeg.x < 0 && dirEnd.y < 0)
				angleOffset = 0;
			else if (dirBeg.x > 0 && dirEnd.y < 0)
				angleOffset = 1;
			else if (dirBeg.x > 0 && dirEnd.y > 0)
				angleOffset = 2;
			else if (dirBeg.x < 0 && dirEnd.y > 0)
				angleOffset = 3;

			origoBeg = vec3(dirBeg.x * 0.5, dirBeg.y * 0.5, radBeg);
			origoEnd = vec3(dirEnd.x * 0.5, dirEnd.y * 0.5, radEnd);

			//get the middle points (z = 0) for the normals
			for (int j = 0; j < res+1; j++)
			{
				rotAngle2 = M_PI / 2 / res * j + angleOffset * M_PI / 2;

				rotAngle *= antiClock;
				pointsBeg1 = vec3(origoBeg.x, origoBeg.y , 0);

				rotAngle *= antiClock;
				pointsEnd1 = vec3(origoEnd.x, origoEnd.y , 0);


				origo = vec3(dirBeg.x * 0.5 + dirEnd.x * 0.5, dirBeg.y * 0.5 + dirEnd.y * 0.5, 0);

				if ((abs(dirBeg.x) > 0 && (abs(dirBeg.x - dirEnd.y) < 0.1)) || (abs(dirBeg.y) > 0 && (abs(dirBeg.y - dirEnd.x) < 0.1))) {
					radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * j * 1.0 / res +
						sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (1 - j * 1.0 / res);
					points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2), 0);
				}
				else
				{
					radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * (1 - j * 1.0 / res) +
						sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (j * 1.0 / res);
					points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2), 0);
				}
				//bottom will be at the same level
				if (radBeg > radEnd)
					points.z += (radEnd - radBeg) * (j * 1.0f / res);
				else
					points.z += (radEnd - radBeg) * (1 - j * 1.0f / res);
				centerPoints.push_back(points);
			}

			//carculate the actual points
			for (int i = 0; i < res + 1; i++)
			{
				//firs and last circles's points (1-without, 2-with offset)
				rotAngle *= antiClock;
				pointsBeg1 = vec3(origoBeg.x + dirBeg.y * sinf(rotAngle * i) * radBeg, origoBeg.y + dirBeg.x * sinf(rotAngle * i) * radBeg, cosf(rotAngle * i) * radBeg);

				rotAngle *= antiClock;
				pointsEnd1 = vec3(origoEnd.x + dirEnd.y * sinf(rotAngle * i) * radEnd, origoEnd.y + dirEnd.x * sinf(rotAngle * i) * radEnd, cosf(rotAngle * i) * radEnd);

				rotAngle *= antiClock;
				pointsBeg2 = vec3(origoBeg.x + dirBeg.y * sinf(rotAngle * (i + 1)) * radBeg, origoBeg.y + dirBeg.x * sinf(rotAngle * (i + 1)) * radBeg, cosf(rotAngle * i) * radBeg);

				rotAngle *= antiClock;
				pointsEnd2 = vec3(origoEnd.x + dirEnd.y * sinf(rotAngle * (i + 1)) * radEnd, origoEnd.y + dirEnd.x * sinf(rotAngle * (i + 1)) * radEnd, cosf(rotAngle * (i + 1)) * radEnd);

				//the pointrs go back and forth 
				if (i % 2) {
					for (int j = 0; j < res + 1; j++)
					{

						rotAngle2 = M_PI / 2 / res * j + angleOffset * M_PI / 2;

						origo = vec3(dirBeg.x * 0.5 + dirEnd.x * 0.5, dirBeg.y * 0.5 + dirEnd.y * 0.5, cosf(rotAngle * i) * radEnd);
						//changes the linear interpolatin starter point
						if ((abs(dirBeg.x) > 0 && (abs(dirBeg.x - dirEnd.y) < 0.1)) || (abs(dirBeg.y) > 0 && (abs(dirBeg.y - dirEnd.x) < 0.1))) {
							radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * j * 1.0 / res +
								sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (1 - j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2), cosf(rotAngle * i) * (radEnd * (1 - j * 1.0 / res) + radBeg * j * 1.0 / res));
						}
						else
						{
							radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * (1 - j * 1.0 / res) +
								sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),  cosf(rotAngle * i) * (radEnd * (j * 1.0 / res) + radBeg * (1 - j * 1.0 / res)));
						}
						if (radBeg > radEnd)
							points.z += (radEnd - radBeg) * (j * 1.0f / res);
						else
							points.z += (radEnd - radBeg) * (1 - j * 1.0f / res);
						//push_back the current point
						vertices.push_back(points);
						normals.push_back(points - centerPoints[j]);
						uvs.push_back(vec2(uvStep * i, uvStep * j));



						origo = vec3(dirBeg.x * 0.5 + dirEnd.x * 0.5, dirBeg.y * 0.5 + dirEnd.y * 0.5, cosf(rotAngle * (i + 1)) * radEnd);
						if ((abs(dirBeg.x) > 0 && (abs(dirBeg.x - dirEnd.y) < 0.1)) || (abs(dirBeg.y) > 0 && (abs(dirBeg.y - dirEnd.x) < 0.1))) {
							radius = sqrt((origo.x - pointsBeg2.x) * (origo.x - pointsBeg2.x) + (origo.y - pointsBeg2.y) * (origo.y - pointsBeg2.y)) * j * 1.0 / res +
								sqrt((origo.x - pointsEnd2.x) * (origo.x - pointsEnd2.x) + (origo.y - pointsEnd2.y) * (origo.y - pointsEnd2.y)) * (1 - j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),  cosf(rotAngle * (i + 1)) * (radEnd * (1 - j * 1.0 / res) + radBeg * j * 1.0 / res));
						}
						else
						{
							radius = sqrt((origo.x - pointsBeg2.x) * (origo.x - pointsBeg2.x) + (origo.y - pointsBeg2.y) * (origo.y - pointsBeg2.y)) * (1 - j * 1.0 / res) +
								sqrt((origo.x - pointsEnd2.x) * (origo.x - pointsEnd2.x) + (origo.y - pointsEnd2.y) * (origo.y - pointsEnd2.y)) * (j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),cosf(rotAngle * (i + 1)) * (radEnd * (j * 1.0 / res) + radBeg * (1 - j * 1.0 / res)));
						}
						if (radBeg > radEnd)
							points.z += (radEnd - radBeg) * (j * 1.0f / res);
						else
							points.z += (radEnd - radBeg) * (1 - j * 1.0f / res);
						//push_back the point in the next raw
						vertices.push_back(points);
						normals.push_back(points - centerPoints[j]);
						uvs.push_back(vec2(uvStep * (i + 1), uvStep * j));
					}
				}
				else {
					for (int j = res; j > -1; j--)
					{

						rotAngle2 = M_PI / 2 / res * j + angleOffset * M_PI / 2;

						origo = vec3(dirBeg.x * 0.5 + dirEnd.x * 0.5, dirBeg.y * 0.5 + dirEnd.y * 0.5, cosf(rotAngle * i) * radEnd);
						if ((abs(dirBeg.x) > 0 && (abs(dirBeg.x - dirEnd.y) < 0.1)) || (abs(dirBeg.y) > 0 && (abs(dirBeg.y - dirEnd.x) < 0.1))) {
							radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * j * 1.0 / res +
								sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (1 - j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),  cosf(rotAngle * i) * (radEnd * (1 - j * 1.0 / res) + radBeg * j * 1.0 / res));
						}
						else
						{
							radius = sqrt((origo.x - pointsBeg1.x) * (origo.x - pointsBeg1.x) + (origo.y - pointsBeg1.y) * (origo.y - pointsBeg1.y)) * (1 - j * 1.0 / res) +
								sqrt((origo.x - pointsEnd1.x) * (origo.x - pointsEnd1.x) + (origo.y - pointsEnd1.y) * (origo.y - pointsEnd1.y)) * (j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2), cosf(rotAngle * i) * (radEnd * (j * 1.0 / res) + radBeg * (1 - j * 1.0 / res)));
						}
						if (radBeg > radEnd)
							points.z += (radEnd - radBeg) * (j * 1.0f / res);
						else
							points.z += (radEnd - radBeg) * (1 - j * 1.0f / res);

						vertices.push_back(points);
						normals.push_back(points - centerPoints[j]);
						uvs.push_back(vec2(uvStep * i, uvStep * j));

						origo = vec3(dirBeg.x * 0.5 + dirEnd.x * 0.5, dirBeg.y * 0.5 + dirEnd.y * 0.5, cosf(rotAngle * (i + 1)) * radEnd);
						if ((abs(dirBeg.x) > 0 && (abs(dirBeg.x - dirEnd.y) < 0.1)) || (abs(dirBeg.y) > 0 && (abs(dirBeg.y - dirEnd.x) < 0.1))) {
							radius = sqrt((origo.x - pointsBeg2.x) * (origo.x - pointsBeg2.x) + (origo.y - pointsBeg2.y) * (origo.y - pointsBeg2.y)) * j * 1.0 / res +
								sqrt((origo.x - pointsEnd2.x) * (origo.x - pointsEnd2.x) + (origo.y - pointsEnd2.y) * (origo.y - pointsEnd2.y)) * (1 - j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),  cosf(rotAngle * (i + 1)) * (radEnd * (1 - j * 1.0 / res) + radBeg * j * 1.0 / res));
						}
						else
						{
							radius = sqrt((origo.x - pointsBeg2.x) * (origo.x - pointsBeg2.x) + (origo.y - pointsBeg2.y) * (origo.y - pointsBeg2.y)) * (1 - j * 1.0 / res) +
								sqrt((origo.x - pointsEnd2.x) * (origo.x - pointsEnd2.x) + (origo.y - pointsEnd2.y) * (origo.y - pointsEnd2.y)) * (j * 1.0 / res);
							points = vec3(origo.x + radius * cosf(rotAngle2), origo.y + radius * sinf(rotAngle2),  cosf(rotAngle * (i + 1)) * (radEnd * (j * 1.0 / res) + radBeg * (1 - j * 1.0 / res)));
						}
						if (radBeg > radEnd)
							points.z += (radEnd - radBeg) * (j * 1.0f / res);
						else
							points.z += (radEnd - radBeg) * (1 - j * 1.0f / res);

						vertices.push_back(points);
						normals.push_back(points - centerPoints[j]);
						uvs.push_back(vec2(uvStep * (i + 1), uvStep * j));
					}
				}
			}
		}
		//if it's a straight pice
		else
		{
			//same piece but in reversed sequence
			if (dirBeg.x < 0 || dirBeg.y < 0)
				reverse = true;
			//it gets the points from the first and last circle's points 
			origoBeg = vec3(dirBeg.x * 0.5, dirBeg.y * 0.5, 0);
			origoEnd = vec3(dirEnd.x * 0.5, dirEnd.y * 0.5, 0);
			
			//to keep the bottom on the same level
			if (radBeg < radEnd)
				origoBeg.z -= (radEnd - radBeg);
			else
				origoEnd.z += (radEnd - radBeg);

			//get the circle's points (back and forth)
			for (int i = 0; i < res + 1; i++)
			{

				rotAngle *= antiClock;
				pointsBeg1 = vec3(origoBeg.x + dirBeg.y * sinf(rotAngle * i) * radBeg, origoBeg.y + dirBeg.x * sinf(rotAngle * i) * radBeg, origoBeg.z + cosf(rotAngle * i) * radBeg);
				vertices.push_back(pointsBeg1);
				uvs.push_back(vec2(uvStep* i, 0));
				normals.push_back(pointsBeg1 - origoBeg);

				rotAngle *= antiClock;
				pointsEnd1 = vec3(origoEnd.x + dirEnd.y * sinf(rotAngle * i) * radEnd, origoEnd.y + dirEnd.x * sinf(rotAngle * i) * radEnd, origoEnd.z + cosf(rotAngle * i) * radEnd);

				vertices.push_back(pointsEnd1);
				uvs.push_back(vec2(uvStep* i, 0));
				normals.push_back(pointsEnd1 - origoEnd);
			}
		}


	}

	virtual void draw() {
		Geometry::draw(GL_TRIANGLE_STRIP);
	}
};