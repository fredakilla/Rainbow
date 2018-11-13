#pragma once

#include "EditorWindow.h"
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QSize>

#include "gameplay.h"


/**
 * The main game view to render scenes into viewport(s).
 */
class GameView : public QWidget, public gameplay::Game
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit GameView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~GameView();

    /**
     * Sets the editor window this game view uses.
     *
     * @param editor The editor window this game view uses.
     */
    void setEditor(EditorWindow* editor);

public slots:

    /**
     * Handler when the scene changes.
     */
    void onSceneChanged();

 protected slots:

     void onTimer();

protected:

    /**
     * @see Game::onInitialize
     */
    void onInitialize();

    /**
     * @see Game::onFinalize
     */
    void onFinalize();

    /**
     * @see Game::onUpdate
     */
    void onUpdate(float elapsedTime);

    /**
     * @see Game::onRender
     */
    void onRender(float elapsedTime);

    /**
     * @see QWidget::paintEvent
     */
    void paintEvent(QPaintEvent* evt);

    /**
     * @see QWidget::resizeEvent
     */
    void resizeEvent(QResizeEvent* evt);

    /**
     * @see QWidget::mousePressEvent
     */
    void mousePressEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseReleaseEvent
     */
    void mouseReleaseEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseMoveEvent
     */
    void mouseMoveEvent(QMouseEvent* evt);

    /**
     * @see QWidget::mouseWheelEvent
     */
    void mouseWheelEvent(QWheelEvent* evt);

    /**
     * @see QWidget::keyPressEvent
     */
    void keyPressEvent(QKeyEvent* evt);

    /**
     * @see QWidget::keyReleaseEvent
     */
    void keyReleaseEvent(QKeyEvent* evt);

    /**
     * @see QWidget::closeEvent
     */
    void closeEvent(QCloseEvent* evt);

private:

    static Input::MouseButton translateMouseButton(Qt::MouseButtons buttons);
    static Input::MouseButton translateMouseButton(Qt::MouseButton button);

    EditorWindow* _editor;
    gameplay::Graphics* _graphics;
    std::shared_ptr<gameplay::Scene> _scene;
    bool _wireframe;
    QTimer _timer;
    QSize _size;
    QPoint _mousePosition;
    double _mouseScroll;
};

