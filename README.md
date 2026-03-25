# yuantan

中石油超远探项目代码备份仓库。

## 项目概览

本项目由高频板和低频板两套板卡构成，整体采用 DSP + FPGA 架构。

- 高频板：负责发射控制以及高频信号相关处理
- 低频板：负责低频信号接收与处理

## 目录说明

- `Cyclone10_FPGA_Code_20260324_tx`：高频板 FPGA 工程
- `Cyclone10_FPGA_Code_20260324_rx`：低频板 FPGA 工程
- `DSP_Code_spfar_t_20260324`：高频板 DSP 工程
- `DSP_Code_spfar_r_20260324`：低频板 DSP 工程

## 说明

- 本仓库主要用于项目代码备份与版本管理
- Quartus、CCS 等工具生成的中间文件已通过 `.gitignore` 排除
- 当前低频相关频点按 `1k / 4k / 20k / 50k` 记录
