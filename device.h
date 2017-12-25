#ifndef DEVICE_H
#define DEVICE_H

#include <list>
#include <memory>

class HidApi {
public:
  static bool init() { static HidApi S; return S.res == 0; }

private:
  HidApi();
  ~HidApi();
  const int res;
};

namespace dcled
{
  class Animation;
  class Screen;

  struct DeviceInfo {
    const uint16_t vendor_id = 0;
    const uint16_t product_id = 0;
    const std::string path;
    const std::string serial_number;
    const std::string manufacturer;
    const std::string product;
  };

  class Device
  {
  public:
    static constexpr uint16_t VENDOR_ID = 0x1d34;
    static constexpr uint16_t PRODUCT_ID = 0x0013;
    static constexpr char EMULATED_DEV_PATH[] = { "/dev/stdout" };

    /// Returns a list of all found dcled devices.
    static const std::list<DeviceInfo> list();

    /// Creates a device instance that opens the first dlced device found.
    Device(bool toStdout = false);
    /// Creates a device instance and tries to open the device via \a device_path.
    Device(const std::string& device_path, bool toStdout = false);

    Device(Device&& other);
    ~Device();

    /// Returns a reference to the Screen, that is sent to the device with \a update() or by the
    /// updater thread.
    Screen& screen();

    /// Returns if the usb device was successfully opened.
    bool isOpen() const;

    /// Send the current screen to the device manually.
    void update();

    /// Add an animation to the queue.
    void enqueue(std::unique_ptr<Animation>&& a);

    /// Play all animations in the queue.
    void playAll();

  private:
    struct Impl;
    std::unique_ptr<Impl> p_;
  };
} // end namespace dcled

#endif // DEVICE_H