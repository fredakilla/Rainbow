#pragma once

#include <rainbow.h>
#include <QWidget>


namespace Ui
{
class PropertiesTransform;
}

/**
 * Transformation properties editor.
 */
class PropertiesTransform : public QWidget
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesTransform(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~PropertiesTransform();

    /**
     * Sets the object to edit properties for.
     *
     * @param object The object to edit transform from.
     */
    void setObject(rainbow::SceneObject* object);

signals:

    void opened();

    void closed();

public slots:

    void onReset(bool);

    void onHeaderPressed();

private:

    void updateProperties();

    Ui::PropertiesTransform* _ui;
    rainbow::SceneObject* _object;
    bool _expanded;
    int _expandedHeight;
};
