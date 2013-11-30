/*
 * 💩
 * Copyright (c) 2013, kumar8600's All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>

const char * g_Command_Attach;
const char * g_Command_Detach;

/*
 * デバイスが接続された時に実行される関数
 */
void func_attached()
{
  printf ("Device attached\n");

  if (g_Command_Attach[0] == '\0')
  {
    return;
  }
  
  int rc = system(g_Command_Attach);
  if (rc != 0)
  {
    fprintf(stderr, "%s : Something went wrong\n", g_Command_Attach);
  }
}

/*
 * デバイスが切断された時に実行される関数
 */
void func_detached()
{
  printf ("Device detached\n");

  if (g_Command_Detach[0] == '\0')
  {
    return;
  }
  
  int rc = system(g_Command_Detach);
  if (rc != 0)
  {
    fprintf(stderr, "%s : Something went wrong\n", g_Command_Detach);
  }
}

/*
 * 終了処理
 */
void finish(int signo)
{
  libusb_exit(NULL);
  puts("Bye");
  exit(EXIT_SUCCESS);
}

/*
 * 指定されたデバイスを見つける関数
 */
libusb_device * find_device(uint16_t vendor_id, uint16_t product_id)
{
  // discover devices
  libusb_device **list;
  libusb_device *found = NULL;
  ssize_t cnt = libusb_get_device_list(NULL, &list);
  ssize_t i = 0;
  if (cnt < 0)
    return NULL;
  for (i = 0; i < cnt; i++)
  {
    libusb_device *device = list[i];
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(device, &desc);
    if (r < 0)
    {
      fprintf(stderr, "failed to get device descriptor");
      continue;
    }
    if (desc.idVendor == vendor_id &&
        desc.idProduct == product_id)
    {
      found = device;
      break;
    }
  }
  
  libusb_free_device_list(list, 1);
  
  return found;
}


/*
 * libusbxに登録する、デバイス接続時のコールバック
 */
static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
  func_attached();
  
  return 0;
}

/*
 * libusbxに登録する、デバイス切断時のコールバック
 */
static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
  func_detached();
  
  return 0;
}

/*
 * つかいかた
 */
void print_usage(char * argv0)
{
  fprintf(stderr, "Usage: %s [-c class_id] [-a attach_command] [-d detach_command] vendor_id product_id\n",
          argv0);
}

int main(int argc, char *argv[])
{
  libusb_hotplug_callback_handle hp[2];
  int product_id, vendor_id, class_id = LIBUSB_HOTPLUG_MATCH_ANY;
  g_Command_Attach = "";
  g_Command_Detach = "";
  int rc;
  char flag_initial_search = 0;
  
  /* getopt */
  while ((rc = getopt(argc, argv, "c:a:d:ih")) != -1) {
    switch (rc) {
      case 'c':
        class_id = strtol(optarg, NULL, 0);
        break;
      case 'a':
        g_Command_Attach = optarg;
        break;
      case 'd':
        g_Command_Detach = optarg;
        break;
      case 'i':
        flag_initial_search = 1;
        break;
      case 'h':
      default: /* '?' */
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (optind + 2 > argc)
  {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  vendor_id = strtol(argv[optind++], NULL, 0);
  product_id = strtol(argv[optind++], NULL, 0);
  
  /* 初期化 */
  rc = libusb_init (NULL);
  if (rc < 0)
  {
    fprintf(stderr, "failed to initialise libusb: %s\n", libusb_error_name(rc));
    return EXIT_FAILURE;
  }

  /* シグナルハンドリング */
  signal(SIGINT, finish);
  signal(SIGTERM, finish);

  /* ホットプラグ対応のプラットフォームか調べる */
  if (!libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG)) {
    fprintf (stderr, "Hotplug capabilites are not supported on this platform\n");
    libusb_exit (NULL);
    return EXIT_FAILURE;
  }

  /* （オプションi）起動した時点での状態から目的のUSBデバイスを探す。 */
  if (flag_initial_search)
  {
    if (find_device(vendor_id, product_id))
    {
      func_attached();
    }
    else
    {
      func_detached();
    }
  }

  /* デバイスが接続されたら実行される関数を登録 */
  rc = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, 0, vendor_id,
                                         product_id, class_id, hotplug_callback, NULL, &hp[0]);
  if (LIBUSB_SUCCESS != rc) {
    fprintf (stderr, "Error registering callback 0\n");
    libusb_exit (NULL);
    return EXIT_FAILURE;
  }

  /* デバイスが切断されたら実行される関数を登録 */
  rc = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, vendor_id,
                                         product_id,class_id, hotplug_callback_detach, NULL, &hp[1]);
  if (LIBUSB_SUCCESS != rc) {
    fprintf (stderr, "Error registering callback 1\n");
    libusb_exit (NULL);
    return EXIT_FAILURE;
  }

  /* メインループ。イベントを回し続ける */
  while (1) {
    rc = libusb_handle_events (NULL);
    if (rc < 0)
      fprintf(stderr, "libusb_handle_events() failed: %s\n", libusb_error_name(rc));
  }

  /* 到達不能コード。終了処理はシグナルにより呼び出される。 */
  /* libusb_exit (NULL); */
}
