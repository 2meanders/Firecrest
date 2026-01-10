#pragma once

#include "PlainGraph.h"
#include "Container.h"
#include "Text.h"
#include "HorisontalCenterer.h"

namespace fc {
    class Graph : public Container {
    public:
        PlainGraph& graph;
        Text& minMaxText;
        ColoredRect& background;

    public:
        Graph(alignment::ElementAlignment alignment, ShapeRenderer2D& shapeRenderer, TextRenderer& textRenderer, float textSize) 
            : Container(alignment)
            , background(createChild<ColoredRect>(
                alignment::ElementAlignment()
                , glm::vec4(0, 0, 0, 1)
                , shapeRenderer
            ))
            , graph(createChild<PlainGraph>(
                alignment::ElementAlignment()
                    .setHeight([textSize, this](float parent1, float parent2) { return parent1 - minMaxText.alignment.height(parent1, parent2); })
                    .setY([this](float parent1, float parent2) { return minMaxText.alignment.height(parent1, parent2); })
                , shapeRenderer
                ))
            , minMaxText(
                createChild<HorisontalCenterer>().
                createChild<Text>(
                    alignment::ElementAlignment()
                    , glm::vec4(1, 1, 1, 1)
                    , textSize
                    , textRenderer
                )) 
            {
                minMaxText._wrapMode = Text::WrapMode::NoWrap;
                minMaxText.wrapTightly = true;
            }

        virtual void render(const Window& window, time::Duration delta) override {
            glDisable(GL_DEPTH_TEST);
            
            background.render(window, delta);
            graph.render(window, delta);
            minMaxText.render(window, delta);
        }
        
        void setData(const std::vector<glm::vec2>& data) {
            graph.data = data;

            if(data.size() == 0) {
                minMaxText.setText("Min: N/A Max: N/A");
                return;
            }
            
            const float dataMinY = std::min_element(data.begin(), data.end(), [](auto a, auto b) { return a.y < b.y; })->y;
            const float dataMaxY = std::max_element(data.begin(), data.end(), [](auto a, auto b) { return a.y < b.y; })->y;

            minMaxText.setText("Min: " + std::to_string(dataMinY) + " Max: " + std::to_string(dataMaxY));
        }
    };
}