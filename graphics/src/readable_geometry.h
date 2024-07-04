#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vectors.h>
#include <geometry.h>
#include <math.h>
#include <GL/glew.h>
using namespace std;

class Readable : public Geometry
{
public:
    Readable(string file)
    {
        std::vector<vec3> rawVertices;
        std::vector<vec3> rawNormals;
        std::vector<vec2> rawUvs;
        string filaname = file, token, token2;
        fstream fs;
        fs.open(filaname);
        string line;
        float x,y,z;
        int counter = 0;
        int counterx = 0, countery = 0, counterz = 0;
        if (!fs.is_open())
        {           
            //throw error
        }
        else
        {
            while (!fs.eof())
            {
                getline(fs, line);
                istringstream iss(line);
                if (line[0] == 'v' && line[1] == ' ')
                {
                    iss >> token;
                    counter = 0;
                    while (iss >> token)
                    {
                        counter++;
                        if (counter > 3)
                        {
                            //error
                        }
                        if(counter == 1)
                            x = stof(token);
                        else if (counter == 2)
                            y = stof(token);
                        else
                            z = stof(token);
                    }
                    rawVertices.push_back(vec3(x, y, z));
                    counterx++;
                }
                else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
                {
                    iss >> token;
                    counter = 0;
                    while (iss >> token)
                    {
                        counter++;
                        if (counter > 3)
                        {
                            //error
                        }
                        else if (counter == 1)
                            x = stof(token);
                        else if (counter == 2)
                            y = stof(token);
                        else
                            z = stof(token);
                    }
                    rawNormals.push_back(vec3(x, y, z));
                    countery++;
                }
                else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
                {
                    iss >> token;
                    counter = 0;
                    while (iss >> token)
                    {
                        counter++;
                        if (counter > 2)
                        {
                            //error
                        }
                        else if (counter == 1)
                            x = stof(token);
                        else 
                            y = stof(token);
                    }
                    rawUvs.push_back(vec2(x, y));
                    counterz++;
                }
                else if (line[0] == 'f' && line[1] == ' ')
                {
                    iss >> token;
                    while (iss >> token)
                    {
                        istringstream iss(token);
                        counter = 0;
                        while (getline(iss, token2, '/')) 
                        {
                            counter++;
                            if (counter > 3)
                            {
                                //error
                            }
                            if (counter == 1)
                                vertices.push_back(rawVertices[stoi(token2)-1]);
                            if (counter == 2)
                                uvs.push_back(rawUvs[stoi(token2)-1]);
                            if (counter == 3)
                                normals.push_back(rawNormals[stoi(token2)-1]);

                        }
                    }
                }
            }
        }
    }
    virtual void draw() {
        Geometry::draw(GL_TRIANGLES);
    }
};



