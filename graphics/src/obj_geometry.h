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

class Obj_geometry : public Geometry
{
private:
    string filename, line;
    string* tokens = new string [3]; //substring storage
    string* tokensf = new string [3]; //substring storage while reading a face
    fstream fs;
    size_t* find = new size_t[3]; //find the positions of the separator characters

    //read a line (line, how may parts do you want, what is the separator character, f is true when reading a face)
    void read(string line, int parts, char ch, bool f)
    {
        for (int i = 0; i < parts; i++)
            find[i] = 0;
        //find the "parts" separator character
        for (int j = 0; j < parts; j++)
        {
            if (!f) {
                if (j)
                    find[j] = line.find(ch, find[j - 1] + 1);
                else
                    find[j] = line.find(ch);
                if(find[j] == string::npos)
                    throw InvalidGeometryException("Not enough data in the line.");
            }
            else
            {
                if (j)
                    find[j] = line.find(ch, find[j - 1] + 1);
                else
                    find[j] = -1;
            }
        }
        if(line.find(ch,find[parts -1]+1) != string::npos)
            throw InvalidGeometryException("Too many data in the line.");
        //store the separated parts in string arrays
        for (int i = 0; i < parts; i++)
        {
            if (!f) {
                if (i == parts - 1)
                    tokens[i] = line.substr(find[i] + 1);
                else
                    tokens[i] = line.substr(find[i] + 1, find[i + 1] - find[i] - 1);
                if (find[i] == string::npos)
                    throw InvalidGeometryException("Not enough data in the line.");
            }
            else
            {
                if (i == parts - 1)
                    tokensf[i] = line.substr(find[i] + 1);
                else
                    tokensf[i] = line.substr(find[i] + 1, find[i + 1] - find[i] - 1);
            }

        }
        if (line.find(ch, find[parts- 1] + 1) != string::npos)
            throw InvalidGeometryException("Too many data in the line.");
    }

    
public:

    Obj_geometry(string file)
    {
        filename = file;
        fs.open(filename);
        vector<vec3> rawVertices, rawNormals; //store the temporary coordinates
        vector<vec2> rawUvs;

        if (!fs.is_open())
            throw InvalidGeometryException("Cannot open the file for the obj_geomerty.");
        while (!fs.eof())
        {
            getline(fs, line);
            istringstream iss(line);
            if (line[0] == 'v' && line[1] == ' ') //read vertices and convert it to vec3
            {
                read(line, 3, ' ', false);
                rawVertices.push_back(vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2])));
            }
            else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') //read normas and convert it to vec3
            {
                read(line, 3, ' ', false);
                rawNormals.push_back(vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2])));
            }
            else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') //read uvs and convert it to vec2
            {
                read(line, 2, ' ', false);
                rawUvs.push_back(vec2(stof(tokens[0]), stof(tokens[1])));
            }
            else if (line[0] == 'f' && line[1] == ' ') //read faces (using raw vectors)
            {
                read(line, 3, ' ', false);
                for (int i = 0; i < 3; i++)
                {
                    read(tokens[i], 3, '/', true);
                    for (int j = 0; j < 3; j++)
                    {
                        if (j == 0)
                            vertices.push_back(rawVertices[stof(tokensf[j]) - 1]);
                        else if (j == 1)
                            uvs.push_back(rawUvs[stof(tokensf[j]) - 1]);
                        else
                            normals.push_back(rawNormals[stof(tokensf[j]) - 1]);
                    }
                }
            }
        }
        
        rawNormals.clear();
        rawUvs.clear();
        rawUvs.clear();
    }
    virtual void draw() {
        Geometry::draw(GL_TRIANGLES);
    }
};



