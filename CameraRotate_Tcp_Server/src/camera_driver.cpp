#include "camera_driver.hpp"
#include <thread>

namespace {

enum {
    kMaxX = 9,
    kMaxY = 4
};

void OperationDelay()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
}

/// The default device driver
class DefaultDriver final : public AbstractCameraDriver  {
public:
    DefaultDriver() :
        x_(0),
        y_(0)
    {}

    void GetMaxPosition(unsigned & maxX, unsigned & maxY) const override
    {
        maxX = kMaxX;
        maxY = kMaxY;
    }

    void GetCurrentPosition(unsigned & x, unsigned & y) const override
    {
        x = x_;
        y = y_;
    }

    void StepLeft() override
    {
        if (x_ < kMaxX) {
            ++x_;
            OperationDelay();
        }

    }

    void StepRight() override
    {
        if (x_ > 0) {
            --x_;
            OperationDelay();
        }
    }

    void StepUp() override
    {
        if (y_ < kMaxY) {
            ++y_;
            OperationDelay();
        }
    }

    void StepDown() override
    {
        if (y_ > 0) {
            --y_;
            OperationDelay();
        }
    }

private:
    int x_;
    int y_;
};

} // namespace

AbstractCameraDriver * GetCameraDriverGlobalInstance()
{
    static DefaultDriver driver;
    return &driver;
}
