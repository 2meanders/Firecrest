#include "firecrest.h"
#include <algorithm>
#include <execution>

using namespace fc;

int main() {
	WindowProperties properties;
	properties.width = 1400;
	properties.height = 800;
	properties.vsync = true;
	properties.resizable = true;
	properties.antialiasing = true;
	properties.title = "UI Example";

	Window window(properties);
	// Create a display
	Display display(window);
	res::ResourceManager res;
	
	ShapeRenderer2D& shapeRenderer = display.createRenderer<ShapeRenderer2D>();
	TextRenderer textRenderer(RESOURCES_PATH "JetBrainsMono-Regular.ttf");
	
	auto& v1 = display.createChild<Scrollable>		(alignment::ElementAlignment().setWidth(alignment::Relative(0.5)).setHeight(alignment::Relative(0.5)), shapeRenderer);
	auto& v2 = display.createChild<Container>		(alignment::ElementAlignment().setWidth(alignment::Relative(0.5)).setHeight(alignment::Relative(0.5)).setY(alignment::Relative(0.5)));
	auto& v3 = display.createChild<Container>		(alignment::ElementAlignment().setWidth(alignment::Relative(0.5)).setHeight(alignment::Relative(0.5)).setX(alignment::Relative(0.5)));
	auto& v4 = display.createChild<Container>		(alignment::ElementAlignment().setWidth(alignment::Relative(0.5)).setHeight(alignment::Relative(0.5)).setX(alignment::Relative(0.5)).setY(alignment::Relative(0.5)));
	
	
	auto& textInput = v1.createChild<TextInput>(
		alignment::ElementAlignment()
		, glm::vec4(1, 1, 1, 1)
		, glm::vec4(0.1, 0.1, 0.1, 1)
		, 60.0f
		, shapeRenderer
		, textRenderer
	);
	textInput._text.verticallyFlexible = true;
	textInput.setText("This is a text input field. Try it!");
	
	
	auto& graph = v2.createChild<Graph>(alignment::ElementAlignment(), shapeRenderer, textRenderer, 16.0f);
	std::vector<glm::vec2> graphData;
	float graphTimeOffset = 0.0f;

	v3.createChild<ColoredRect>(alignment::ElementAlignment(), glm::vec4(0, 0, 0, 1), shapeRenderer);
	v4.createChild<ColoredRect>(alignment::ElementAlignment(), glm::vec4(0.05, 0.05, 0.05, 1), shapeRenderer);

	auto& textbox = v3
	.createChild<VerticalCenterer>()
	.createChild<HorisontalCenterer>()
	.createChild<Text>(
		alignment::ElementAlignment().setWidth(alignment::Relative(0.8f)),
		glm::vec4(1, 1, 1, 1),
		14.0f,
		textRenderer
	);
	// textbox.growToFitText = false;
	textbox.setText("This is a text box. It can contain multiple lines of text.\n"
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
		"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
		"Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
		"Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
	);
	textbox.verticallyFlexible = true;
	textbox.horisontallyFlexible = true;

	auto& button = v4
	.createChild<VerticalCenterer>()
	.createChild<HorisontalCenterer>()
	.createChild<Button>(
		alignment::ElementAlignment().setWidth(alignment::Pixels(200)).setHeight(alignment::Relative(0.5f)), 
		glm::vec4(1, 0, 0, 1), 
		glm::vec4(0.8, 0, 0, 1), 
		glm::vec4(0.5, 0, 0, 1), 
		glm::vec4(1, 1, 1, 1), 
		16.0f, 
		[]() { 
			std::cout << "Pressed! Callback called" << std::endl; 
		},
		shapeRenderer, 
		textRenderer
	);
	button.text.setText("Press me!");

	// Draw lightsource
	window.clearColor(glm::vec4(0.05, 0.05, 0.05, 1.0f));
	glm::vec2 lastMousePos = window.getInput().mouse();

	// Create a display
	window.clearColor(glm::vec4(1, 0, 1, 1.0f));
	time::Moment lastTime = time::now();
	while (!window.shouldClose()) {
		time::Moment now = time::now();
		time::Duration delta = now - lastTime;
		lastTime = now;
		
		graphData.push_back({graphTimeOffset, sin(graphTimeOffset) * graphTimeOffset});
		graph.setData(graphData);
		graphTimeOffset += 0.15f;

		window.clearScreen();
		display.render();
		window.display();
		// Include performance metrics in the title
		window.setTitle(std::string("UI Example ("
			+ std::to_string(delta.millis()) + "ms"
			+ " fps: "
			+ std::to_string((int)maths::round(1.0f / static_cast<float>(delta.seconds()), 1))
			+ ")"
		));
	}
	
	return 0;
}