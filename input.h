#ifndef INPUT_H
#define INPUT_H

#include <Qt>
#include <QPoint>

class Input
{
public:

    enum InputState { InputInvalid, InputRegistered, InputUnregistered, InputTriggered, InputPressed, InputReleased };

    static InputState keyState(Qt::Key key);
    static bool keyTriggered(Qt::Key key);
    static bool keyPressed(Qt::Key key);
    static bool keyReleased(Qt::Key key);
    static InputState buttonState(Qt::MouseButton button);
    static bool buttonTriggered(Qt::MouseButton button);
    static bool buttonPressed(Qt::MouseButton button);
    static bool buttonReleased(Qt::MouseButton button);
    static QPoint mousePosition();
    static QPoint mouseDelta();

private:
    static void update();
    static void registerKeyPressed(int key);
    static void registerKeyReleased(int key);
    static void registerMousePressed(Qt::MouseButton button);
    static void registerMouseReleased(Qt::MouseButton button);
    static void reset();
    friend class Window;

    Input();
};


inline bool Input::keyTriggered(Qt::Key key) { return keyState(key) == InputTriggered; }
inline bool Input::keyPressed(Qt::Key key) { return keyState(key) == InputPressed; }
inline bool Input::keyReleased(Qt::Key key) { return keyState(key) == InputReleased; }
inline bool Input::buttonTriggered(Qt::MouseButton button) { return buttonState(button) == InputTriggered; }
inline bool Input::buttonPressed(Qt::MouseButton button) { return buttonState(button) == InputPressed; }
inline bool Input::buttonReleased(Qt::MouseButton button) { return buttonState(button) == InputReleased; }

#endif // INPUT_H
