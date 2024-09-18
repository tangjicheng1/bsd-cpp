#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <iostream>

namespace nb = nanobind;

class Application {
   public:
    virtual ~Application() = default;
    virtual void onOpen() = 0;
    virtual void onMessage(int val) = 0;
    virtual void onClose() = 0;
};

class BindedApplication : public Application {
   public:
    NB_TRAMPOLINE(Application, sizeof(Application));

    void onOpen() override {
        NB_OVERRIDE_PURE(onOpen);
    }

    // Define trampoline for onMessage
    void onMessage(int val) override {
        NB_OVERRIDE_PURE(onMessage, val);
    }

    // Define trampoline for onClose
    void onClose() override {
        NB_OVERRIDE_PURE(onClose);
    }
};

class Runner {
   public:
    Runner(Application& app) : app_(app) {
    }
    void run(int val) {
        app_.onOpen();
        app_.onMessage(val);
        app_.onClose();
    }

   private:
    Application& app_;
};

NB_MODULE(demo3, m) {
    // Binding for Application class
    nb::class_<Application, BindedApplication>(m, "Application")
        .def(nb::init<>())  // Default constructor
        .def("onOpen", &Application::onOpen)
        .def("onMessage", &Application::onMessage)
        .def("onClose", &Application::onClose);

    // Binding for Runner class
    nb::class_<Runner>(m, "Runner").def(nb::init<Application&>()).def("run", &Runner::run);
}
