#include "Mytec/app.hpp"
#include "Mytec/util/vec.hpp"

#include <imgui-SFML.h>


namespace mytec
{
app::app(const char *title, const v2u size, const v2u min_size, sf::Image &&icon, style &&style)
    : window(sf::VideoMode{size}, title), images(window.getNativeHandle()), style_(std::move(style))
{
    window.setMinimumSize(min_size);
    window.setVerticalSyncEnabled(true);
    window.setIcon(std::move(icon));
    window.requestFocus();

    assert(ImGui::SFML::Init(window, false) && "Could not initialize ImGui context...");
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF("../../fonts/pixChicago.ttf", 15);
    assert(ImGui::SFML::UpdateFontTexture() && "Could not load font...");
}

app::~app()
{
    ImGui::SFML::Shutdown(window);
}

void app::process_events()
{
    while (const auto opt_ev = window.pollEvent())
    {
        const sf::Event &ev = opt_ev.value();
        ImGui::SFML::ProcessEvent(window, ev);

        if (ev.is<sf::Event::Closed>())
        {
            want_close = true;
        }
        else if (const auto resized = ev.getIf<sf::Event::Resized>())
        {
            const auto size = resized->size;
            window.setView({v2f{}, v2f{size}});
            window.setSize(size);
        }
    }
}

void app::run()
{
    window.resetGLStates();
    style_.set_active();

    // https://github.com/SFML/imgui-sfml/issues/206#issuecomment-2239907726
    ImGui::SFML::ProcessEvent(window, sf::Event::FocusLost{});
    ImGui::SFML::ProcessEvent(window, sf::Event::FocusGained{});
    sf::Clock dt;
    while (window.isOpen())
    {
        process_events();
        ImGui::SFML::Update(window, dt.restart());

        if (want_close && images.try_close(want_close))
        {
            window.close();
        }
        render();
    }
}

auto app::render_menu() -> void
{
    using namespace ImGui;
    if (BeginMainMenuBar())
    {
        if (BeginMenu("Demo"))
        {
            if (MenuItem("Show demo", nullptr, nullptr))
            {
                if (show_demo)
                {
                    SetWindowFocus("Dear ImGui Demo");
                }
                show_demo = true;
            }
            EndMenu();
        }
        EndMainMenuBar();
    }
}

auto app::render() -> void
{
    window.clear(style_.bg);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style_.bg);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
    {
        images.update();
        render_menu();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    if (show_demo)
    {
        ImGui::ShowDemoWindow(&show_demo);
    }

    ImGui::SFML::Render(window);
    window.display();
}
}
