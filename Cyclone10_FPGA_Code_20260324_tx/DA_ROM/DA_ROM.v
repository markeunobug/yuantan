module DA_ROM
(
    input        clk_10m,       // 10MHz 时钟
    input        clk_1m,        // 1MHz 时钟
    input        sys_rst,       // 复位，低有效
    
    output [15:0] sinrom_1k,    // 1kHz 正弦波（16位输出）
    output [15:0] sinrom_4k,
    output [15:0] sinrom_20k,
    output [15:0] sinrom_50k,
    output [15:0] sinrom_400k,
    output [15:0] sinrom_2M     // 2MHz 正弦波（16位输出）
);

//-----------------------------------------------------------------
// 地址寄存器（10位宽，对应 0~999，1000点表）
//-----------------------------------------------------------------
reg [9:0] addr_1k;
reg [9:0] addr_4k;
reg [9:0] addr_20k;
reg [9:0] addr_50k;
reg [9:0] addr_400k;
reg [9:0] addr_2M;

//-----------------------------------------------------------------
// 低频信号地址累加（1MHz 时钟）
// 频率 = (1MHz × 步长) / 1000
//-----------------------------------------------------------------
always @(posedge clk_1m or negedge sys_rst) begin
    if (!sys_rst) begin
        addr_1k   <= 10'd0;
        addr_4k   <= 10'd0;
        addr_20k  <= 10'd0;
        addr_50k  <= 10'd0;
    end
    else begin
        addr_1k   <= (addr_1k   + 10'd1)   % 10'd1000;   // 1kHz
        addr_4k   <= (addr_4k   + 10'd4)   % 10'd1000;   // 4kHz
        addr_20k  <= (addr_20k  + 10'd20)  % 10'd1000;   // 20kHz
        addr_50k  <= (addr_50k  + 10'd50)  % 10'd1000;   // 50kHz
    end
end

//-----------------------------------------------------------------
// 高频信号地址累加（10MHz 时钟）
// 频率 = (10MHz × 步长) / 1000
//-----------------------------------------------------------------
always @(posedge clk_10m or negedge sys_rst) begin
    if (!sys_rst) begin
        addr_400k <= 10'd0;
        addr_2M   <= 10'd0;
    end
    else begin
        addr_400k <= (addr_400k + 10'd40)  % 10'd1000;   // 400kHz
        addr_2M   <= (addr_2M   + 10'd200) % 10'd1000;   // 2MHz
    end
end

//-----------------------------------------------------------------
// ROM 例化（16位输出）
// 注意：你需要重新生成/替换 IP 核，使其输出宽度为 16 位
// 低频用 clk_1m，高频用 clk_10m
//-----------------------------------------------------------------

DA_ROM_SIN_IP sin_rom_1k (    // ← 请改成你实际的16位IP核名字
    .clock    (clk_1m),
    .address  (addr_1k),
    .q        (sinrom_1k)
);

DA_ROM_SIN_IP sin_rom_4k (
    .clock    (clk_1m),
    .address  (addr_4k),
    .q        (sinrom_4k)
);

DA_ROM_SIN_IP sin_rom_20k (
    .clock    (clk_1m),
    .address  (addr_20k),
    .q        (sinrom_20k)
);

DA_ROM_SIN_IP sin_rom_50k (
    .clock    (clk_1m),
    .address  (addr_50k),
    .q        (sinrom_50k)
);

DA_ROM_SIN_IP sin_rom_400k (
    .clock    (clk_10m),
    .address  (addr_400k),
    .q        (sinrom_400k)
);

DA_ROM_SIN_IP sin_rom_2M (
    .clock    (clk_10m),
    .address  (addr_2M),
    .q        (sinrom_2M)
);

endmodule