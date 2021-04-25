#include <SFML/Graphics.hpp>
#include "dirent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "MaxRectsBinPack.h"
#include "Image.h"

const char* toStr(size_t value) {
    std::string str = std::to_string(value);
    return str.c_str();
}

std::vector<std::string> getListFiles(std::string dirName) {
    DIR *dir;
    std::vector<std::string> list;
    struct dirent *ent;
    std::string filename = dirName;
    if ((dir = opendir(filename.c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            std::string str = ent->d_name;

            if (str != "." && str != "..")
                list.push_back(str);
        }
        closedir(dir);
    }
    return list;
}

rbp::MaxRectsBinPack::FreeRectChoiceHeuristic chooseBestHeuristic(std::vector<sf::Texture*> *rects, size_t texWidth, size_t texHeight) {
    rbp::MaxRectsBinPack pack;
    std::vector<rbp::MaxRectsBinPack::FreeRectChoiceHeuristic> listHeuristics;
    listHeuristics.push_back(rbp::MaxRectsBinPack::RectBestAreaFit);
    listHeuristics.push_back(rbp::MaxRectsBinPack::RectBestLongSideFit);
    listHeuristics.push_back(rbp::MaxRectsBinPack::RectBestShortSideFit);
    listHeuristics.push_back(rbp::MaxRectsBinPack::RectBottomLeftRule);
    listHeuristics.push_back(rbp::MaxRectsBinPack::RectContactPointRule);

    rbp::MaxRectsBinPack::FreeRectChoiceHeuristic res;
    float max = 0;

    for (auto& heu : listHeuristics) {
        pack.Init(texWidth, texHeight);

        for (size_t j = 0; j < rects->size(); j++) {
            pack.Insert(rects->at(j)->getSize().x, rects->at(j)->getSize().y, heu);
        }

        if (pack.Occupancy() > max) {
            max = pack.Occupancy();
            res = heu;
        }
    }
    return res;
}


int main(int argc, char** argv) {
    std::string folderName;
    if (argc != 2) 
    {
        std::cout << "Folder name expected as argument" << std::endl;
        return -1;
    }
    else
    {
        folderName = argv[1];
    }
    
    std::vector<sf::Texture*> imgTex; // images textures
    std::vector<std::string> imgTexID; // name of the images
    std::vector<Image> images; // xml data of the images
    std::string filename = "sheet"; // filename of the sprite sheet
    sf::Vector2i size(2048, 2048); // size of the sprite sheet

    sf::RenderTexture rend; // texture to render the sprite sheet
    rend.create(size.x, size.y);

    rbp::MaxRectsBinPack pack(size.x, size.y); //pack of image

    // list all filenames in the folder images
    std::vector<std::string> listAll = getListFiles(folderName);
    
    if (!listAll.size())
    {
        std::cout << "No files found in " << folderName << std::endl;
        return -1;
    }
    for (auto name : listAll)
    {
        std::cout << "file: " << name << std::endl;
    }

    // load all the images
    for (auto& img : listAll) {
        sf::Texture *texP = new sf::Texture();
        texP->loadFromFile(folderName + img);
        imgTex.push_back(texP);
        imgTexID.push_back(img.substr(0, listAll.size() - 4));
    }

    float rotation = 0;

    // choose the best heuristic
    const rbp::MaxRectsBinPack::FreeRectChoiceHeuristic best1 = chooseBestHeuristic(&imgTex, size.x, size.y);

    for (size_t i = 0; i < imgTex.size(); i++) {
        // insert the image into the pack
        rbp::Rect packedRect = pack.Insert(imgTex[i]->getSize().x, imgTex[i]->getSize().y, best1);

        if (packedRect.height <= 0) {
            std::cout << "Error: The pack is full\n";
        }

        sf::Sprite spr(*imgTex[i]); // sprite to draw on the rendertexture

        // if the image is rotated
        if (imgTex[i]->getSize().x == packedRect.height && packedRect.width != packedRect.height) {
            rotation = 90; // set the rotation for the xml data

            // rotate the sprite to draw
            size_t oldHeight = spr.getTextureRect().height;
            spr.setPosition((float) packedRect.x, (float) packedRect.y);
            spr.rotate(rotation);
            spr.setPosition(spr.getPosition().x + oldHeight, spr.getPosition().y);
        }
        else { // if there is no rotation
            rotation = 0;
            spr.setPosition((float) packedRect.x, (float) packedRect.y);
        }

        rend.draw(spr); // draw the sprite on the sprite sheet
        // save data of the image for the xml file
        images.push_back(Image(filename, imgTexID[i], packedRect.x, packedRect.y, packedRect.width, packedRect.height, (size_t)rotation));
    }

    rend.display(); // render the texture properly

    // free the memory of the images
    for (auto& tex : imgTex) {
        delete(tex);
    }

    // save the sprite sheet
    sf::Texture tex = rend.getTexture();
    sf::Image img = tex.copyToImage(); // need to create an image to save a file
    img.saveToFile("sheets/" + filename + ".png");


    // see the occupancy of the packing
    std::cout << "pack1 : " << pack.Occupancy() << "%\n";
    
    // SFML code the create a window and diplay the sprite sheet
    sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sprite sheets generator");
    sf::Sprite spr(tex);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(spr);
        window.display();
    }
}
