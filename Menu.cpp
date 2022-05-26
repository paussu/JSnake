//
// Created by jipe on 5/8/21.
//
#include "Menu.h"
#include "Game.h"
#include "Hiscores.h"
#include "Options.h"

Menu::Menu()
: isRunning(true), gameStartRequested(false), showMenu(true), showOptions(false), mDisplay(nullptr)
, mEventQueue(nullptr), clearColor(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)), mWidth(1280), mHeight(720)
{
    mOptions = std::make_unique<Options>(this);
    mHiscores = std::make_unique<Hiscores>(this);
}

Menu::~Menu()
{
}

bool Menu::Initialize()
{
    // Setup Allegro
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    mDisplay = al_create_display(mWidth, mHeight);
    al_set_window_position(mDisplay, mWidth / 2, mHeight / 2);
    al_set_window_title(mDisplay, "JSnake");

    mEventQueue = al_create_event_queue();
    al_register_event_source(mEventQueue, al_get_display_event_source(mDisplay));
    al_register_event_source(mEventQueue, al_get_keyboard_event_source());
    al_register_event_source(mEventQueue, al_get_mouse_event_source());

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAllegro5_Init(mDisplay);

    mHiscores->LoadFromFile();

    return true;
}

void Menu::RunLoop()
{
    while (isRunning)
    {
        ProcessInput();
        GenerateOutput();

        if (gameStartRequested)
        {
            gameStartRequested = false;

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
            isRunning = false;
        if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            ImGui_ImplAllegro5_InvalidateDeviceObjects();
            al_acknowledge_resize(mDisplay);
            ImGui_ImplAllegro5_CreateDeviceObjects();
        }
    }
}

void Menu::GenerateOutput()
{
    // Start the Dear ImGui frame
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    DrawMenu();
    mOptions->Draw();
    mHiscores->Draw();

    // Rendering
    ImGui::Render();
    al_clear_to_color(al_map_rgba_f(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w));
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
    al_flip_display();
}

void Menu::DrawMenu()
{
    ImGui::SetNextWindowSize(ImVec2(mWidth  / 4.0, mHeight - (mHeight / 2.0)), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(mWidth / 3.0, 100));
    ImGui::Begin("Start menu", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Play", ImVec2(mWidth  / 4.0, 100)))
        gameStartRequested = true;

    if (ImGui::Button("Options", ImVec2(mWidth  / 4.0, 100)))
        mOptions->SetShown();

    if (ImGui::Button("Hiscores", ImVec2(mWidth  / 4.0, 100)))
        mHiscores->SetShown();

    if (ImGui::Button("Exit", ImVec2(mWidth  / 4.0, 100)))
        isRunning = false;

    ImGui::End();
}

void Menu::RunGame()
{
    auto game = std::make_unique<Game>(&mOptions->GetGameConfiguration());
    int success = game->Initialize();
    if (success)
    {
        game->RunLoop();

        // Save score after game is over
        mHiscores->SaveScore(game->GetPlayerName(), game->GetScore());
    }

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

