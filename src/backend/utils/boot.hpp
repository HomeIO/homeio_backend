#ifndef BOOT_HPP
#define BOOT_HPP

#include "../io/io_server.hpp"
#include "../meas/meas_fetcher.hpp"
#include "../overseer/overseer_array.hpp"
#include "../addons/addons_array.hpp"
#include "../utils/spy.hpp"
#include "../meas/meas_buffer_backup_storage.hpp"
#include "../meas/file_storage.hpp"
#include "../tcp/tcp_server.hpp"

class Boot {
 public:
  Boot();

  bool beginShutdown;

  std::shared_ptr<Spy> spy;
  std::shared_ptr<MeasFetcher> measFetcher;
  std::shared_ptr<TcpServer> tcpServer;
  std::shared_ptr<MeasBufferBackupStorage> measBufferBackupStorage;
  std::shared_ptr<IoServer> ioServer;
  std::shared_ptr<OverseerArray> overseerArray;
  std::shared_ptr<FileStorage> fileStorage;
  std::shared_ptr<AddonsArray> addonsArray;

};

#endif
