#include "InputManager.h"

InputManager* InputManager::s_Instance = NULL;

InputManager* InputManager::GetInstance()
{
    if (s_Instance == NULL) {
        s_Instance = new InputManager();
    }
    return s_Instance;
}

bool InputManager::KeyDown(SDL_Scancode scancode)
{
    //Tecla foi apertada
    return m_KeyboardState[scancode];
}

bool InputManager::KeyPressed(SDL_Scancode scancode)
{
    //Foi apertada e antes não tava apertada
    return m_KeyboardState[scancode] && !m_PrevKeyboardState[scancode];
}

bool InputManager::KeyReleased(SDL_Scancode scancode)
{
    //não ta apertada e antes tava
    return !m_KeyboardState[scancode] && m_PrevKeyboardState[scancode];;
}

std::tuple<int, int> InputManager::MousePos()
{
    return { m_MouseX, m_MouseY };
}

void InputManager::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT || m_KeyboardState[SDL_SCANCODE_ESCAPE]) quitFlag = true;
    }
    m_MouseState = SDL_GetRelativeMouseState(&m_MouseX, &m_MouseY);
}

void InputManager::UpdatePreviousState()
{
    memcpy(m_PrevKeyboardState, m_KeyboardState, m_KeyLength);
    m_PrevMouseState = m_MouseState;
}

bool InputManager::MouseButtonDown(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button) {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    }

    return (m_MouseState & mask);
}

bool InputManager::MouseButtonPressed(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button) {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    }

    return (m_MouseState & mask) && !(m_PrevMouseState & mask);
}

bool InputManager::MouseButtonReleased(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button) {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    }

    return !(m_MouseState & mask) && (m_PrevMouseState & mask);
}

InputManager::InputManager()
{
    //Basicamente inicializando os estados do teclado, e colocando o estado antigo = o atual pra começar
    m_KeyboardState = SDL_GetKeyboardState(&m_KeyLength);
    m_PrevKeyboardState = new Uint8[m_KeyLength];
    memcpy(m_PrevKeyboardState, m_KeyboardState, m_KeyLength);
}

InputManager::~InputManager()
{
    delete[] m_KeyboardState;
    m_PrevKeyboardState = NULL;
}

