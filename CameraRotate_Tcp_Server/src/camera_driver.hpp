#pragma once

class AbstractCameraDriver {
public:
    virtual ~AbstractCameraDriver() {}

    /// Возвращает координаты крайней левой/верхней позиции
    virtual void GetMaxPosition(unsigned & maxX, unsigned & maxY) const = 0;

    /// Возвращает координаты текущей позиции
    virtual void GetCurrentPosition(unsigned & x, unsigned & y) const = 0;

    /// Осуществляет поворот на одну позцию в заданном направлении.
    /// Блокирует вызывающий поток до завершения выполнения. При достижении
    /// граничной позиции ничего не делает и сразу возвращает управление.
    virtual void StepLeft() = 0;
    virtual void StepRight() = 0;
    virtual void StepUp() = 0;
    virtual void StepDown() = 0;
};

/// Возвращает указатель на глобальный объект (синглтон), реализующий интерфейс
/// управления поворотным модулем. Время жизни объекта гарантировано до завершения
/// функции main().
AbstractCameraDriver * GetCameraDriverGlobalInstance();
