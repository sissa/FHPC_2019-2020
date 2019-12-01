## Evaluating   HPC nodes

In this short tutorial we report some procedure to evaluate/understand the kind of computational nodes we have at disposal and which kind of performance we can obtain.



### Explore the structure of the node

- Identify the CPU
```bash
>cat /proc/cpuinfo | grep model  | sort | uniq -1
model		: 85
model name	: Intel(R) Xeon(R) Gold 6126 CPU @ 2.60GHz
```
We can also use lscpu to get more information: 

```bash
>lscpu
lscpu
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                24
On-line CPU(s) list:   0-23
Thread(s) per core:    1
Core(s) per socket:    12
Socket(s):             2
NUMA node(s):          2
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 85
Model name:            Intel(R) Xeon(R) Gold 6126 CPU @ 2.60GHz
Stepping:              4
CPU MHz:               2600.000
BogoMIPS:              5200.00
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              1024K
L3 cache:              19712K
NUMA node0 CPU(s):     0,2,4,6,8,10,12,14,16,18,20,22
NUMA node1 CPU(s):     1,3,5,7,9,11,13,15,17,19,21,23
Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc aperfmperf eagerfpu pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch epb cat_l3 cdp_l3 intel_ppin intel_pt ssbd mba ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts pku ospke spec_ctrl intel_stibp flush_l1d
```

 - how much RAM ? 
```bash
>free -h
              total        used        free      shared  buff/cache   available
Mem:           754G        5.1G        748G         29M        492M        747G
Swap:           31G          0B         31G
```

- Does it have Infiniband ? 

  ```bash
   >lspci | grep Mellanox
  3b:00.0 Infiniband controller: Mellanox Technologies MT28908 Family [ConnectX-6]
  ```
- What is the overall NUMA architecture?
 ```bash
 [root@ct1pt-tnode001 bin]# numactl --hardware
available: 2 nodes (0-1)
node 0 cpus: 0 2 4 6 8 10 12 14 16 18 20 22
node 0 size: 391640 MB
node 0 free: 381953 MB
node 1 cpus: 1 3 5 7 9 11 13 15 17 19 21 23
node 1 size: 393216 MB
node 1 free: 384331 MB
node distances:
node   0   1
  0:  10  21
  1:  21  10
 ```




### Using  hwloc  tool 

```bash
[root@ct1pt-tnode001 bin]# ./hwloc-gather-topology --io --dmi /tmp/node1

*** Note that this tool may be slow on large nodes or when I/O is enabled. ***

Gathering main files and directories...
 file /proc/cmdline
 file /proc/cpuinfo
 file /proc/meminfo
 file /proc/mounts
 file /proc/stat
 file /proc/version
 file /proc/self/cpuset
 file /proc/self/cgroup
 file /proc/driver/nvidia
 directory /sys/devices/system/cpu/
 directory /sys/bus/cpu/devices/
 directory /sys/devices/system/node/
 directory /sys/bus/node/devices/
 directory /sys/class/dmi/id/
 directory /sys/devices/virtual/dmi/id/
 directory /sys/kernel/mm/hugepages/
 directory /proc/device-tree/cpus/
 directory /sys/fs/cgroup/cpuset/
Gathering I/O files...
 directory /sys/bus/pci/devices/
 directory /sys/bus/pci/slots/
 directory /sys/devices/pci0000:00
 directory /sys/devices/pci0000:17
 directory /sys/devices/pci0000:3a
 directory /sys/devices/pci0000:5d
 directory /sys/devices/pci0000:80
 directory /sys/devices/pci0000:85
 directory /sys/devices/pci0000:ae
 directory /sys/devices/pci0000:d7
 directory /sys/class/block/
 directory /sys/class/dma/
 directory /sys/class/drm/
 directory /sys/class/drm/../../devices/virtual/drm/ttm
 directory /sys/class/infiniband/
 directory /sys/class/net/
 directory /sys/class/net/../../devices/virtual/net/em1.712
 directory /sys/class/net/../../devices/virtual/net/lo
 file /run/udev/data/b8:0
 file /run/udev/data/b8:1
 file /run/udev/data/b8:2
 file /run/udev/data/b8:3
 file /run/udev/data/b8:4
Gathering DMI files...
 directory /sys/firmware/dmi/
Exporting /proc/hwloc-nofile-info

Exporting x86 CPUID using hwloc-gather-cpuid

Topology files gathered in /tmp/node1.tar.bz2
Expected topology output stored in [root@ct1pt-tnode001 bin]# ./hwloc-gather-topology --io --dmi /tmp/node1

*** Note that this tool may be slow on large nodes or when I/O is enabled. ***

Gathering main files and directories...
 file /proc/cmdline
 file /proc/cpuinfo
 file /proc/meminfo
 file /proc/mounts
 file /proc/stat
 file /proc/version
 file /proc/self/cpuset
 file /proc/self/cgroup
 file /proc/driver/nvidia
 directory /sys/devices/system/cpu/
 directory /sys/bus/cpu/devices/
 directory /sys/devices/system/node/
 directory /sys/bus/node/devices/
 directory /sys/class/dmi/id/
 directory /sys/devices/virtual/dmi/id/
 directory /sys/kernel/mm/hugepages/
 directory /proc/device-tree/cpus/
 directory /sys/fs/cgroup/cpuset/
Gathering I/O files...
 directory /sys/bus/pci/devices/
 directory /sys/bus/pci/slots/
 directory /sys/devices/pci0000:00
 directory /sys/devices/pci0000:17
 directory /sys/devices/pci0000:3a
 directory /sys/devices/pci0000:5d
 directory /sys/devices/pci0000:80
 directory /sys/devices/pci0000:85
 directory /sys/devices/pci0000:ae
 directory /sys/devices/pci0000:d7
 directory /sys/class/block/
 directory /sys/class/dma/
 directory /sys/class/drm/
 directory /sys/class/drm/../../devices/virtual/drm/ttm
 directory /sys/class/infiniband/
 directory /sys/class/net/
 directory /sys/class/net/../../devices/virtual/net/em1.712
 directory /sys/class/net/../../devices/virtual/net/lo
 file /run/udev/data/b8:0
 file /run/udev/data/b8:1
 file /run/udev/data/b8:2
 file /run/udev/data/b8:3
 file /run/udev/data/b8:4
Gathering DMI files...
 directory /sys/firmware/dmi/
Exporting /proc/hwloc-nofile-info

Exporting x86 CPUID using hwloc-gather-cpuid

Topology files gathered in /tmp/node1.tar.bz2
Expected topology output stored in /tmp/node1.output
 
cat  /tmp/node1.output
[root@ct1pt-tnode001 bin]# cat /tmp/node1.output
Machine (P#0 total=803693408KB DMIProductName="PowerEdge R640" DMIProductVersion= DMIProductSerial=CYVMVY2 DMIProductUUID=4C4C4544-0059-5610-804D-C3C04F565932 DMIBoardVendor="Dell Inc." DMIBoardName=0RGP26 DMIBoardVersion=A01 DMIBoardSerial=.CYVMVY2.CNIVC009560629. DMIBoardAssetTag= DMIChassisVendor="Dell Inc." DMIChassisType=23 DMIChassisVersion= DMIChassisSerial=CYVMVY2 DMIChassisAssetTag= DMIBIOSVendor="Dell Inc." DMIBIOSVersion=2.2.11 DMIBIOSDate=06/13/2019 DMISysVendor="Dell Inc." Backend=Linux LinuxCgroup=/ hwlocVersion=2.1.0 ProcessName=lstopo-no-graphics)
  Package L#0 (P#0 total=401040224KB CPUModel="Intel(R) Xeon(R) Gold 6126 CPU @ 2.60GHz" CPUVendor=GenuineIntel CPUFamilyNumber=6 CPUModelNumber=85 CPUStepping=4)
    NUMANode L#0 (P#0 local=401040224KB total=401040224KB)
    L3Cache L#0 (size=19712KB linesize=64 ways=11 Inclusive=0)
      L2Cache L#0 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#0 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#0 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#0 (P#0)
              PU L#0 (P#0)
      L2Cache L#1 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#1 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#1 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#1 (P#6)
              PU L#1 (P#2)
      L2Cache L#2 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#2 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#2 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#2 (P#2)
              PU L#2 (P#4)
      L2Cache L#3 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#3 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#3 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#3 (P#5)
              PU L#3 (P#6)
      L2Cache L#4 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#4 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#4 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#4 (P#3)
              PU L#4 (P#8)
      L2Cache L#5 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#5 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#5 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#5 (P#4)
              PU L#5 (P#10)
      L2Cache L#6 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#6 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#6 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#6 (P#8)
              PU L#6 (P#12)
      L2Cache L#7 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#7 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#7 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#7 (P#14)
              PU L#7 (P#14)
      L2Cache L#8 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#8 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#8 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#8 (P#9)
              PU L#8 (P#16)
      L2Cache L#9 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#9 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#9 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#9 (P#12)
              PU L#9 (P#18)
      L2Cache L#10 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#10 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#10 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#10 (P#10)
              PU L#10 (P#20)
      L2Cache L#11 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#11 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#11 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#11 (P#11)
              PU L#11 (P#22)
    HostBridge L#0 (buses=0000:[00-03])
      PCI L#0 (busid=0000:00:11.5 id=8086:a1d2 class=0106(SATA) PCIVendor="Intel Corporation" PCIDevice="C620 Series Chipset Family SSATA Controller [AHCI mode]")
      PCI L#1 (busid=0000:00:17.0 id=8086:a182 class=0106(SATA) PCIVendor="Intel Corporation" PCIDevice="C620 Series Chipset Family SATA Controller [AHCI mode]")
      PCIBridge L#1 (busid=0000:00:1c.4 id=8086:a194 class=0604(PCIBridge) link=0.50GB/s buses=0000:[02-03] PCIVendor="Intel Corporation" PCIDevice="C620 Series Chipset Family PCI Express Root Port #5")
        PCIBridge L#2 (busid=0000:02:00.0 id=1556:be00 class=0604(PCIBridge) link=0.50GB/s buses=0000:[03-03] PCIVendor=PLDA)
          PCI L#2 (busid=0000:03:00.0 id=102b:0536 class=0300(VGA) PCIVendor="Matrox Electronics Systems Ltd." PCIDevice="Integrated Matrox G200eW3 Graphics Controller")
    HostBridge L#3 (buses=0000:[17-19])
      PCIBridge L#4 (busid=0000:17:00.0 id=8086:2030 class=0604(PCIBridge) link=7.88GB/s buses=0000:[18-18] PCISlot=0 PCIVendor="Intel Corporation" PCIDevice="Sky Lake-E PCI Express Root Port A")
        PCI L#3 (busid=0000:18:00.0 id=1000:005f class=0104(RAID) link=7.88GB/s PCIVendor="LSI Logic / Symbios Logic" PCIDevice="MegaRAID SAS-3 3008 [Fury]")
          Block(Disk) L#0 (Size=468320256 SectorSize=512 LinuxDeviceID=8:0 Vendor=DELL Model=PERC_H330_Mini Revision=4.29 SerialNumber=64cd98f0321df50024ddfc4711885f01) "sda"
      PCIBridge L#5 (busid=0000:17:02.0 id=8086:2032 class=0604(PCIBridge) link=7.88GB/s buses=0000:[19-19] PCIVendor="Intel Corporation" PCIDevice="Sky Lake-E PCI Express Root Port C")
        PCI L#4 (busid=0000:19:00.0 id=1077:8070 class=0200(Ethernet) link=7.88GB/s PCIVendor="QLogic Corp." PCIDevice="FastLinQ QL41000 Series 10/25/40/50GbE Controller")
          Network L#1 (Address=34:80:0d:4e:52:74) "em1"
        PCI L#5 (busid=0000:19:00.1 id=1077:8070 class=0200(Ethernet) link=7.88GB/s PCIVendor="QLogic Corp." PCIDevice="FastLinQ QL41000 Series 10/25/40/50GbE Controller")
          Network L#2 (Address=34:80:0d:4e:52:75) "em2"
    HostBridge L#6 (buses=0000:[3a-3b])
      PCIBridge L#7 (busid=0000:3a:00.0 id=8086:2030 class=0604(PCIBridge) link=15.75GB/s buses=0000:[3b-3b] PCISlot=1 PCIVendor="Intel Corporation" PCIDevice="Sky Lake-E PCI Express Root Port A")
        PCI L#6 (busid=0000:3b:00.0 id=15b3:101b class=0207(InfiniBand) link=15.75GB/s PCIVendor="Mellanox Technologies" PCIDevice="MT28908 Family [ConnectX-6]")
          Network L#3 (Address=20:00:10:86:fe:80:00:00:00:00:00:00:b8:59:9f:03:00:d4:27:e2 Port=1) "ib0"
          OpenFabrics L#4 (NodeGUID=b859:9f03:00d4:27e2 SysImageGUID=b859:9f03:00d4:27e2 Port1State=4 Port1LID=0x15 Port1LMC=0 Port1GID0=fe80:0000:0000:0000:b859:9f03:00d4:27e2) "mlx5_0"
  Package L#1 (P#1 total=402653184KB CPUModel="Intel(R) Xeon(R) Gold 6126 CPU @ 2.60GHz" CPUVendor=GenuineIntel CPUFamilyNumber=6 CPUModelNumber=85 CPUStepping=4)
    NUMANode L#1 (P#1 local=402653184KB total=402653184KB)
    L3Cache L#1 (size=19712KB linesize=64 ways=11 Inclusive=0)
      L2Cache L#12 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#12 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#12 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#12 (P#0)
              PU L#12 (P#1)
      L2Cache L#13 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#13 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#13 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#13 (P#6)
              PU L#13 (P#3)
      L2Cache L#14 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#14 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#14 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#14 (P#2)
              PU L#14 (P#5)
      L2Cache L#15 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#15 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#15 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#15 (P#5)
              PU L#15 (P#7)
      L2Cache L#16 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#16 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#16 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#16 (P#3)
              PU L#16 (P#9)
      L2Cache L#17 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#17 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#17 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#17 (P#4)
              PU L#17 (P#11)
      L2Cache L#18 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#18 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#18 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#18 (P#8)
              PU L#18 (P#13)
      L2Cache L#19 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#19 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#19 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#19 (P#14)
              PU L#19 (P#15)
      L2Cache L#20 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#20 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#20 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#20 (P#9)
              PU L#20 (P#17)
      L2Cache L#21 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#21 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#21 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#21 (P#12)
              PU L#21 (P#19)
      L2Cache L#22 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#22 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#22 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#22 (P#10)
              PU L#22 (P#21)
      L2Cache L#23 (size=1024KB linesize=64 ways=16 Inclusive=0)
        L1dCache L#23 (size=32KB linesize=64 ways=8 Inclusive=0)
          L1iCache L#23 (size=32KB linesize=64 ways=8 Inclusive=0)
            Core L#23 (P#11)
              PU L#23 (P#23)
  Misc(MemoryModule) L#0 (P#0 DeviceLocation=A1 Vendor=00CE00B300CE SerialNumber=246FBB42 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#1 (P#1 DeviceLocation=A2 Vendor=00CE00B300CE SerialNumber=246FBA6C AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#2 (P#2 DeviceLocation=A3 Vendor=00CE00B300CE SerialNumber=246FBB43 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#3 (P#3 DeviceLocation=A4 Vendor=00CE00B300CE SerialNumber=246FBA69 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#4 (P#4 DeviceLocation=A5 Vendor=00CE00B300CE SerialNumber=246FC5D5 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#5 (P#5 DeviceLocation=A6 Vendor=00CE00B300CE SerialNumber=246FBA6D AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#6 (P#12 DeviceLocation=B1 Vendor=00CE00B300CE SerialNumber=246FBDC6 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#7 (P#13 DeviceLocation=B2 Vendor=00CE00B300CE SerialNumber=246FBD05 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#8 (P#14 DeviceLocation=B3 Vendor=00CE00B300CE SerialNumber=246FC211 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#9 (P#15 DeviceLocation=B4 Vendor=00CE00B300CE SerialNumber=246FC2B5 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#10 (P#16 DeviceLocation=B5 Vendor=00CE00B300CE SerialNumber=246FBCC0 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
  Misc(MemoryModule) L#11 (P#17 DeviceLocation=B6 Vendor=00CE00B300CE SerialNumber=246FBC72 AssetTag=03185051 PartNumber=M386A8K40BM2-CTD)
depth 0:           1 Machine (type #0)
 depth 1:          2 Package (type #1)
  depth 2:         2 L3Cache (type #6)
   depth 3:        24 L2Cache (type #5)
    depth 4:       24 L1dCache (type #4)
     depth 5:      24 L1iCache (type #9)
      depth 6:     24 Core (type #2)
       depth 7:    24 PU (type #3)
Special depth -3:  2 NUMANode (type #13)
Special depth -4:  8 Bridge (type #14)
Special depth -5:  7 PCIDev (type #15)
Special depth -6:  5 OSDev (type #16)
Special depth -7:  12 Misc (type #17)
Relative latency matrix (name NUMALatency kind 5) between 2 NUMANodes (depth -3) by logical indexes:
  index     0     1
      0    10    21
      1    21    10
Topology not from this system
```



### Low level performance on Infiniband cards

Using the low level performance tools coming with Infiniband software 

```bash
[root@ct1pt-tnode002 ~]#  ib_send_lat

************************************
* Waiting for client to connect... *
************************************
---------------------------------------------------------------------------------------
                    Send Latency Test
 Dual-port       : OFF		Device         : mlx5_0
 Number of qps   : 1		Transport type : IB
 Connection type : RC		Using SRQ      : OFF
 RX depth        : 512
 Mtu             : 4096[B]
 Link type       : IB
 Max inline data : 236[B]
 rdma_cm QPs	 : OFF
 Data ex. method : Ethernet
---------------------------------------------------------------------------------------
 local address: LID 0x0a QPN 0x0214 PSN 0xc7eeb1
 remote address: LID 0x15 QPN 0x05e8 PSN 0x7a9568
---------------------------------------------------------------------------------------
 #bytes #iterations    t_min[usec]    t_max[usec]  t_typical[usec]    t_avg[usec]    t_stdev[usec]   99% percentile[usec]   99.9% percentile[usec]
 2       1000          0.96           3.45         1.00     	       1.01        	0.09   		1.06    		3.45
---------------------------------------------------------------------------------------

[root@ct1pt-tnode001 benchmarks]# ib_read_bw ct1pt-tnode002
---------------------------------------------------------------------------------------
                    RDMA_Read BW Test
 Dual-port       : OFF		Device         : mlx5_0
 Number of qps   : 1		Transport type : IB
 Connection type : RC		Using SRQ      : OFF
 TX depth        : 128
 CQ Moderation   : 100
 Mtu             : 4096[B]
 Link type       : IB
 Outstand reads  : 16
 rdma_cm QPs	 : OFF
 Data ex. method : Ethernet
---------------------------------------------------------------------------------------
 local address: LID 0x15 QPN 0x060f PSN 0x766fd0 OUT 0x10 RKey 0x00d1dc VAddr 0x007fdc14e70000
 remote address: LID 0x0a QPN 0x023b PSN 0x774ffe OUT 0x10 RKey 0x007225 VAddr 0x007f60f4ab0000
---------------------------------------------------------------------------------------
 #bytes     #iterations    BW peak[MB/sec]    BW average[MB/sec]   MsgRate[Mpps]
 65536      1000             11709.18            11708.31		   0.187333
---------------------------------------------------------------------------------------
```

### 

### Measure network performance: MPI with Intel 

 - On the same node, two cores one on socket 0, the other on socket 1 

```bash
[root@ct1pt-tnode001 benchmarks]# mpirun -np 2 -ppn=2 -f machines -env I_MPI_DEBUG 5 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 /opt/arkitech/intel/impi/2019.5.281/benchmarks/imb/src_c/IMB-MPI1 PingPong
[0] MPI startup(): libfabric version: 1.7.2a-impi
[0] MPI startup(): libfabric provider: verbs;ofi_rxm
[0] MPI startup(): Rank    Pid      Node name       Pin cpu
[0] MPI startup(): 0       575459   ct1pt-tnode001  0
[0] MPI startup(): 1       575460   ct1pt-tnode001  1
[0] MPI startup(): I_MPI_ROOT=/opt/arkitech/intel/compilers_and_libraries_2019.5.281/linux/mpi
[0] MPI startup(): I_MPI_MPIRUN=mpirun
[0] MPI startup(): I_MPI_HYDRA_TOPOLIB=hwloc
[0] MPI startup(): I_MPI_PIN_PROCESSOR_LIST=0,1
[0] MPI startup(): I_MPI_INTERNAL_MEM_POLICY=default
[0] MPI startup(): I_MPI_DEBUG=5
#------------------------------------------------------------
#    Intel(R) MPI Benchmarks 2018, MPI-1 part
#------------------------------------------------------------
# Date                  : Sat Nov 16 12:58:51 2019
# Machine               : x86_64
# System                : Linux
# Release               : 3.10.0-957.el7.x86_64
# Version               : #1 SMP Thu Nov 8 23:39:32 UTC 2018
# MPI Version           : 3.1
# MPI Thread Environment:


# Calling sequence was:

# /opt/arkitech/intel/impi/2019.5.281/benchmarks/imb/src_c/IMB-MPI1 PingPong

# Minimum message length in bytes:   0
# Maximum message length in bytes:   4194304
#
# MPI_Datatype                   :   MPI_BYTE
# MPI_Datatype for reductions    :   MPI_FLOAT
# MPI_Op                         :   MPI_SUM
#
#

# List of Benchmarks to run:

# PingPong

#---------------------------------------------------
# Benchmarking PingPong
# #processes = 2
#---------------------------------------------------
       #bytes #repetitions      t[usec]   Mbytes/sec
            0         1000         0.42         0.00
            1         1000         0.42         2.38
            2         1000         0.42         4.76
            4         1000         0.42         9.48
            8         1000         0.42        19.08
           16         1000         0.42        37.99
           32         1000         0.44        73.07
           64         1000         0.44       146.17
          128         1000         0.54       238.00
          256         1000         0.61       419.74
          512         1000         0.79       648.47
         1024         1000         0.86      1187.67
         2048         1000         1.02      2001.62
         4096         1000         1.44      2851.23
         8192         1000         1.98      4144.24
        16384         1000         3.33      4920.65
        32768         1000         5.10      6422.96
        65536          640         9.34      7016.31
       131072          320        17.76      7380.71
       262144          160        34.08      7691.05
       524288           80        82.88      6325.76
      1048576           40       168.04      6240.07
      2097152           20       334.37      6271.90
      4194304           10       664.89      6308.24


# All processes entering MPI_Finalize
```

- On the same node, two cores one on socket 0


```bash
[root@ct1pt-tnode001 benchmarks]# mpirun -np 2 -ppn=2 -f machines -env I_MPI_DEBUG 5 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 /opt/arkitech/intel/impi/2019.5.281/benchmarks/imb/src_c/IMB-MPI1 PingPong
[0] MPI startup(): libfabric version: 1.7.2a-impi
[0] MPI startup(): libfabric provider: verbs;ofi_rxm
[0] MPI startup(): Rank    Pid      Node name       Pin cpu
[0] MPI startup(): 0       576752   ct1pt-tnode001  0
[0] MPI startup(): 1       576753   ct1pt-tnode001  2
[0] MPI startup(): I_MPI_ROOT=/opt/arkitech/intel/compilers_and_libraries_2019.5.281/linux/mpi
[0] MPI startup(): I_MPI_MPIRUN=mpirun
[0] MPI startup(): I_MPI_HYDRA_TOPOLIB=hwloc
[0] MPI startup(): I_MPI_PIN_PROCESSOR_LIST=0,2
[0] MPI startup(): I_MPI_INTERNAL_MEM_POLICY=default
[0] MPI startup(): I_MPI_DEBUG=5
#------------------------------------------------------------
#    Intel(R) MPI Benchmarks 2018, MPI-1 part
#------------------------------------------------------------
# Date                  : Sat Nov 16 13:08:56 2019
# Machine               : x86_64
# System                : Linux
# Release               : 3.10.0-957.el7.x86_64
# Version               : #1 SMP Thu Nov 8 23:39:32 UTC 2018
# MPI Version           : 3.1
# MPI Thread Environment:


# Calling sequence was:

# /opt/arkitech/intel/impi/2019.5.281/benchmarks/imb/src_c/IMB-MPI1 PingPong

# Minimum message length in bytes:   0
# Maximum message length in bytes:   4194304
#
# MPI_Datatype                   :   MPI_BYTE
# MPI_Datatype for reductions    :   MPI_FLOAT
# MPI_Op                         :   MPI_SUM
#
#

# List of Benchmarks to run:

# PingPong

#---------------------------------------------------
# Benchmarking PingPong
# #processes = 2
#---------------------------------------------------
       #bytes #repetitions      t[usec]   Mbytes/sec
            0         1000         0.23         0.00
            1         1000         0.23         4.27
            2         1000         0.23         8.75
            4         1000         0.23        17.52
            8         1000         0.23        35.08
           16         1000         0.23        70.07
           32         1000         0.24       134.95
           64         1000         0.24       269.28
          128         1000         0.26       492.35
          256         1000         0.29       893.26
          512         1000         0.45      1128.02
         1024         1000         0.56      1824.45
         2048         1000         0.67      3077.27
         4096         1000         0.86      4740.76
         8192         1000         1.40      5858.46
        16384         1000         2.46      6663.00
        32768         1000         3.79      8648.37
        65536          640         6.09     10760.49
       131072          320         9.37     13989.19
       262144          160        16.23     16155.37
       524288           80        42.86     12232.03
      1048576           40        83.53     12552.81
      2097152           20       158.96     13192.63
      4194304           10       417.05     10057.01


# All processes entering MPI_Finalize
```



### OpenMPI

Using the low level performance tools


```bash
[root@ct1pt-tnode002 ~]#  ib_send_lat

************************************
* Waiting for client to connect... *
************************************
---------------------------------------------------------------------------------------
                    Send Latency Test
 Dual-port       : OFF		Device         : mlx5_0
 Number of qps   : 1		Transport type : IB
 Connection type : RC		Using SRQ      : OFF
 RX depth        : 512
 Mtu             : 4096[B]
 Link type       : IB
 Max inline data : 236[B]
 rdma_cm QPs	 : OFF
 Data ex. method : Ethernet
---------------------------------------------------------------------------------------
 local address: LID 0x0a QPN 0x0214 PSN 0xc7eeb1
 remote address: LID 0x15 QPN 0x05e8 PSN 0x7a9568
---------------------------------------------------------------------------------------
 #bytes #iterations    t_min[usec]    t_max[usec]  t_typical[usec]    t_avg[usec]    t_stdev[usec]   99% percentile[usec]   99.9% percentile[usec]
 2       1000          0.96           3.45         1.00     	       1.01        	0.09   		1.06    		3.45
---------------------------------------------------------------------------------------

[root@ct1pt-tnode001 benchmarks]# ib_read_bw ct1pt-tnode002
---------------------------------------------------------------------------------------
                    RDMA_Read BW Test
 Dual-port       : OFF		Device         : mlx5_0
 Number of qps   : 1		Transport type : IB
 Connection type : RC		Using SRQ      : OFF
 TX depth        : 128
 CQ Moderation   : 100
 Mtu             : 4096[B]
 Link type       : IB
 Outstand reads  : 16
 rdma_cm QPs	 : OFF
 Data ex. method : Ethernet
---------------------------------------------------------------------------------------
 local address: LID 0x15 QPN 0x060f PSN 0x766fd0 OUT 0x10 RKey 0x00d1dc VAddr 0x007fdc14e70000
 remote address: LID 0x0a QPN 0x023b PSN 0x774ffe OUT 0x10 RKey 0x007225 VAddr 0x007f60f4ab0000
---------------------------------------------------------------------------------------
 #bytes     #iterations    BW peak[MB/sec]    BW average[MB/sec]   MsgRate[Mpps]
 65536      1000             11709.18            11708.31		   0.187333
---------------------------------------------------------------------------------------
```

### OpenMPI

 - OpenMPI within node:

```
[root@ct1pt-tnode002 IMB_src_c]# mpirun --allow-run-as-root -np 2 -npersocket 1  --report-bindings  ./IMB-MPI1 PingPong
[ct1pt-tnode002:586563] MCW rank 0 bound to socket 0[core 0[hwt 0]]: [B/././././././././././.][./././././././././././.]
[ct1pt-tnode002:586563] MCW rank 1 bound to socket 1[core 12[hwt 0]]: [./././././././././././.][B/././././././././././.]
#------------------------------------------------------------
#    Intel(R) MPI Benchmarks 2018, MPI-1 part
#------------------------------------------------------------
# Date                  : Sat Nov 16 14:20:42 2019
# Machine               : x86_64
# System                : Linux
# Release               : 3.10.0-957.el7.x86_64
# Version               : #1 SMP Thu Nov 8 23:39:32 UTC 2018
# MPI Version           : 3.1
# MPI Thread Environment:


# Calling sequence was:

# ./IMB-MPI1 PingPong

# Minimum message length in bytes:   0
# Maximum message length in bytes:   4194304
#
# MPI_Datatype                   :   MPI_BYTE
# MPI_Datatype for reductions    :   MPI_FLOAT
# MPI_Op                         :   MPI_SUM
#
#

# List of Benchmarks to run:

# PingPong

#---------------------------------------------------
# Benchmarking PingPong
# #processes = 2
#---------------------------------------------------
       #bytes #repetitions      t[usec]   Mbytes/sec
            0         1000         0.60         0.00
            1         1000         0.70         1.43
            2         1000         0.69         2.89
            4         1000         0.69         5.79
            8         1000         0.69        11.54
           16         1000         0.74        21.70
           32         1000         0.77        41.48
           64         1000         0.93        69.15
          128         1000         1.09       117.02
          256         1000         1.27       201.43
          512         1000         1.28       401.17
         1024         1000         1.52       673.90
         2048         1000         1.86      1100.10
         4096         1000         3.23      1269.12
         8192         1000         3.56      2301.96
        16384         1000         4.23      3869.06
        32768         1000         5.31      6174.82
        65536          640         7.19      9115.32
       131072          320        10.56     12408.69
       262144          160        17.16     15278.45
       524288           80        34.39     15246.43
      1048576           40        93.84     11173.63
      2097152           20       220.01      9532.02
      4194304           10       437.48      9587.35


# All processes entering MPI_Finalize
```


 -   - OpenMPI across nodes:

```
[root@ct1pt-tnode002 IMB_src_c]# mpirun --allow-run-as-root -np 2 --report-bindings -host ct1pt-tnode002,ct1pt-tnode003  -npernode 1 ./IMB-MPI1 PingPong
[ct1pt-tnode002:586414] MCW rank 0 bound to socket 0[core 0[hwt 0]]: [B/././././././././././.][./././././././././././.]
[ct1pt-tnode003:586895] MCW rank 1 bound to socket 0[core 0[hwt 0]]: [B/././././././././././.][./././././././././././.]
#------------------------------------------------------------
#    Intel(R) MPI Benchmarks 2018, MPI-1 part
#------------------------------------------------------------
# Date                  : Sat Nov 16 14:18:30 2019
# Machine               : x86_64
# System                : Linux
# Release               : 3.10.0-957.el7.x86_64
# Version               : #1 SMP Thu Nov 8 23:39:32 UTC 2018
# MPI Version           : 3.1
# MPI Thread Environment:


# Calling sequence was:

# ./IMB-MPI1 PingPong

# Minimum message length in bytes:   0
# Maximum message length in bytes:   4194304
#
# MPI_Datatype                   :   MPI_BYTE
# MPI_Datatype for reductions    :   MPI_FLOAT
# MPI_Op                         :   MPI_SUM
#
#

# List of Benchmarks to run:

# PingPong

#---------------------------------------------------
# Benchmarking PingPong
# #processes = 2
#---------------------------------------------------
       #bytes #repetitions      t[usec]   Mbytes/sec
            0         1000        14.13         0.00
            1         1000        14.21         0.07
            2         1000        14.21         0.14
            4         1000        14.21         0.28
            8         1000        14.21         0.56
           16         1000        14.18         1.13
           32         1000        14.21         2.25
           64         1000        14.24         4.49
          128         1000        14.28         8.96
          256         1000        14.71        17.40
          512         1000        14.84        34.49
         1024         1000        15.23        67.25
         2048         1000        34.25        59.79
         4096         1000        34.97       117.14
         8192         1000        36.46       224.70
        16384         1000        39.41       415.75
        32768         1000        48.94       669.59
        65536          640        70.81       925.58
       131072          320       125.11      1047.62
       262144          160       147.60      1776.06
       524288           80       273.45      1917.29
      1048576           40       380.20      2757.95
      2097152           20       671.35      3123.79
      4194304           10      1474.58      2844.40


# All processes entering MPI_Finalize
```

 - openMPI with infiniband enabled 
```bash
[root@ct1pt-tnode002 arkitech]# /opt/arkitech/openmpi-3.1.4/bin/mpirun --allow-run-as-root -np 2  -host ct1pt-tnode002,ct1pt-tnode003 /opt/arkitech/benchmarks/IMB_src_c/IMB-MPI1  PingPong
--------------------------------------------------------------------------
WARNING: No preset parameters were found for the device that Open MPI
detected:

  Local host:            ct1pt-tnode002
  Device name:           mlx5_0
  Device vendor ID:      0x02c9
  Device vendor part ID: 4123

Default device parameters will be used, which may result in lower
performance.  You can edit any of the files specified by the
btl_openib_device_param_files MCA parameter to set values for your
device.

NOTE: You can turn off this warning by setting the MCA parameter
      btl_openib_warn_no_device_params_found to 0.
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   ct1pt-tnode002
  Local device: mlx5_0
--------------------------------------------------------------------------
#------------------------------------------------------------
#    Intel(R) MPI Benchmarks 2018, MPI-1 part
#------------------------------------------------------------
# Date                  : Sat Nov 16 17:04:57 2019
# Machine               : x86_64
# System                : Linux
# Release               : 3.10.0-957.el7.x86_64
# Version               : #1 SMP Thu Nov 8 23:39:32 UTC 2018
# MPI Version           : 3.1
# MPI Thread Environment:


# Calling sequence was:

# /opt/arkitech/benchmarks/IMB_src_c/IMB-MPI1 PingPong

# Minimum message length in bytes:   0
# Maximum message length in bytes:   4194304
#
# MPI_Datatype                   :   MPI_BYTE
# MPI_Datatype for reductions    :   MPI_FLOAT
# MPI_Op                         :   MPI_SUM
#
#

# List of Benchmarks to run:

# PingPong

#---------------------------------------------------
# Benchmarking PingPong
# #processes = 2
#---------------------------------------------------
       #bytes #repetitions      t[usec]   Mbytes/sec
            0         1000         0.98         0.00
            1         1000         0.98         1.02
            2         1000         0.99         2.02
            4         1000         0.99         4.05
            8         1000         0.99         8.12
           16         1000         0.99        16.21
           32         1000         1.01        31.76
           64         1000         1.19        53.88
          128         1000         1.24       103.41
          256         1000         1.44       178.00
          512         1000         1.50       340.64
         1024         1000         1.65       618.94
         2048         1000         2.34       876.62
         4096         1000         3.26      1254.60
         8192         1000         4.80      1707.01
        16384         1000         6.51      2516.34
        32768         1000         8.96      3655.55
        65536          640        12.95      5062.61
       131072          320        19.54      6708.07
       262144          160        32.74      8006.39
       524288           80        55.77      9401.11
      1048576           40        98.83     10609.62
      2097152           20       183.22     11446.10
      4194304           10       354.38     11835.63
```






