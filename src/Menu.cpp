#include "Menu.h"
#include "Game.h"
#include "Hiscores.h"
#include "Logger.h"
#include "Options.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"

#include <allegro5/allegro_primitives.h>
#include <algorithm>

Menu::~Menu() = default;
namespace
{
    const ImVec4 kTitleColor(0.98f, 0.91f, 0.39f, 1.0f);
    const ImVec4 kAccentColor(0.27f, 0.80f, 0.62f, 1.0f);
}

Menu::Menu()
{
    mOptions = std::make_unique<Options>(this);
    mHiscores = std::make_unique<Hiscores>(this);
}

bool Menu::Initialize()
{
    Logger::Debug("Initializing main menu");

    // Setup Allegro
    if (!al_init())
    {
        Logger::Error("Failed to initialize Allegro");
        return false;
    }

    if (!al_install_keyboard())
        Logger::Error("Failed to install keyboard input");

    if (!al_install_mouse())
        Logger::Error("Failed to install mouse input");

    al_init_primitives_addon();

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    mDisplay = al_create_display(mWidth, mHeight);
    if (mDisplay == nullptr)
    {
        Logger::Error("Failed to create menu display");
        return false;
    }

    al_set_window_position(mDisplay, mWidth / 2, mHeight / 2);
    al_set_window_title(mDisplay, "JSnake");

    mEventQueue = al_create_event_queue();
    if (mEventQueue == nullptr)
    {
        Logger::Error("Failed to create menu event queue");
        return false;
    }

    al_register_event_source(mEventQueue, al_get_display_event_source(mDisplay));
    al_register_event_source(mEventQueue, al_get_keyboard_event_source());
    al_register_event_source(mEventQueue, al_get_mouse_event_source());

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 18.0f;
    style.ChildRounding = 14.0f;
    style.FrameRounding = 10.0f;
    style.PopupRounding = 12.0f;
    style.GrabRounding = 10.0f;
    style.ScrollbarRounding = 12.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(12.0f, 12.0f);
    style.ItemInnerSpacing = ImVec2(10.0f, 8.0f);
    style.WindowPadding = ImVec2(20.0f, 20.0f);

    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.09f, 0.12f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.12f, 0.16f, 0.85f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.24f, 0.31f, 0.25f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.58f, 0.49f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.72f, 0.60f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.17f, 0.49f, 0.41f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.44f, 0.70f, 0.75f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.23f, 0.55f, 0.86f, 0.85f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.39f, 0.63f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.14f, 0.19f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.19f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.22f, 0.29f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.07f, 0.10f, 0.95f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.09f, 0.12f, 0.95f);

    // Setup Platform/Renderer backends
    ImGui_ImplAllegro5_Init(mDisplay);

    mHiscores->LoadFromFile();
    Logger::Debug("Main menu initialized successfully");

    return true;
}

void Menu::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        GenerateOutput();

        if (mGameStartRequested)
        {
            mGameStartRequested = false;

            // Exit menu
            Shutdown();

            // Run game
            RunGame();

            // Come back and initialize the menu again
            Initialize();
        }
    }
}

void Menu::Shutdown()
{
    Logger::Debug("Shutting down main menu");

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(mEventQueue);
    al_destroy_display(mDisplay);
}

void Menu::ProcessInput()
{
    // Poll and handle events (inputs, window resize, etc.)
    ALLEGRO_EVENT ev;
    while (al_get_next_event(mEventQueue, &ev))
    {
        ImGui_ImplAllegro5_ProcessEvent(&ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            mIsRunning = false;

        if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            ImGui_ImplAllegro5_InvalidateDeviceObjects();
            al_acknowledge_resize(mDisplay);
            ImGui_ImplAllegro5_CreateDeviceObjects();
            mWidth = al_get_display_width(mDisplay);
            mHeight = al_get_display_height(mDisplay);
        }
    }
}

void Menu::GenerateOutput()
{
    // Start the Dear ImGui frame
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    DrawMenuBackground(mWidth, mHeight);
    DrawMenu();
    mOptions->Draw();
    mHiscores->Draw();

    // Rendering
    ImGui::Render();
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
    al_flip_display();
}

void Menu::DrawMenu()
{
    const auto &config = mOptions->GetGameConfiguration();
    const float panelWidth = std::min(mWidth * 0.36f, 430.0f);
    const float panelHeight = std::min(mHeight * 0.85f, 600.0f);
    const ImVec2 panelSize(panelWidth, panelHeight);

    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2((mWidth - panelWidth) * 0.5f, (mHeight - panelHeight) * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.96f);
    ImGui::Begin("Start menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    ImGui::TextColored(kTitleColor, "JSnake");
    ImGui::TextDisabled("Retro snake with a cleaner front-end.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("QuickSummary", ImVec2(0.0f, 130.0f), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextColored(kAccentColor, "Current setup");
    ImGui::BulletText("Resolution: %dx%d", config.screenWidth, config.screenHeight);
    ImGui::BulletText("Display mode: %s", config.fullscreen ? "Fullscreen" : "Windowed");
    ImGui::BulletText("Style: %s", config.useSprites ? "Sprite graphics" : "Classic blocks");
    ImGui::Spacing();
    ImGui::TextWrapped("Tip: Use arrow keys to move, P to pause, and ESC to leave a run.");
    ImGui::EndChild();

    ImGui::Spacing();

    const ImVec2 buttonSize(-1.0f, 54.0f);
    if (ImGui::Button("Play game", buttonSize))
        mGameStartRequested = true;

    if (ImGui::Button("Options", buttonSize))
        mOptions->SetShown();

    if (ImGui::Button("Hiscores", buttonSize))
        mHiscores->SetShown();

    if (ImGui::Button("Exit", buttonSize))
        mIsRunning = false;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextDisabled("Built with Allegro5 and Dear ImGui");

    ImGui::End();
}

void Menu::DrawMenuBackground(int width, int height)
{
    const float screenWidth = static_cast<float>(width);
    const float screenHeight = static_cast<float>(height);

    al_clear_to_color(al_map_rgb(12, 16, 26));
    al_draw_filled_rectangle(0.0f, 0.0f, screenWidth, screenHeight, al_map_rgb(10, 14, 22));

    al_draw_filled_rectangle(0.0f, 0.0f, screenWidth, screenHeight * 0.30f, al_map_rgba(26, 40, 61, 235));
    al_draw_filled_triangle(screenWidth * 0.52f, 0.0f, screenWidth, 0.0f, screenWidth, screenHeight * 0.46f, al_map_rgba(24, 76, 66, 72));
    al_draw_filled_triangle(0.0f, screenHeight, 0.0f, screenHeight * 0.52f, screenWidth * 0.38f, screenHeight, al_map_rgba(21, 54, 86, 58));

    al_draw_filled_circle(screenWidth * 0.84f, screenHeight * 0.18f, screenHeight * 0.20f, al_map_rgba(58, 201, 157, 22));
    al_draw_filled_circle(screenWidth * 0.18f, screenHeight * 0.84f, screenHeight * 0.15f, al_map_rgba(49, 120, 201, 18));
    al_draw_filled_circle(screenWidth * 0.72f, screenHeight * 0.72f, screenHeight * 0.11f, al_map_rgba(245, 226, 97, 10));
}

void Menu::RunGame()
{
    Logger::Debug("Starting game from main menu");

    auto game = std::make_unique<Game>(&mOptions->GetGameConfiguration());
    bool success = game->Initialize();
    if (success)
    {
        game->RunLoop();

        // Save score after game is over
        mHiscores->SaveScore(game->GetPlayerName(), game->GetScore());
        Logger::Debug("Game session finished successfully");
    }
    else
        Logger::Error("Game initialization failed from menu");

    game->Shutdown();
}

int Menu::GetWidth() const
{
    return mWidth;
}

int Menu::GetHeight() const
{
    return mHeight;
}

