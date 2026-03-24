module DAC7821(
    input           sys_clk,
    input           sys_rstn,
    //数据
    input [3:0]     Amp,                // DSP控制的幅度字 
    input [7:0]     ROM_Data,           //码表
    //硬件管脚
    output reg[11:0]DAC7821_PIN,        
    output          DAC7821_CS  
);

wire [11:0] DAC7821_Data;
// 用來給 SignalTap 觀察的 register
reg  [11:0] DAC_Data_reg;           // ← 新增這一行
reg  [3:0] DAC_AMP_reg;           // ← 新增這一行
reg  [7:0] DAC_RAWData_reg;           // ← 新增這一行

DA_Mul_IP	DA_Mul_IP_inst (
	.dataa ( ROM_Data ),
	.datab ( Amp ),
	.result ( DAC7821_Data )
	);

// 直接把乘法器輸出打到 reg 裡（跟隨 wire 的變化）
always @(posedge sys_clk) begin     // 用 posedge 比較保險
    DAC_Data_reg <= DAC7821_Data;
    DAC_AMP_reg <= Amp;
    DAC_RAWData_reg <= ROM_Data;  
end

assign DAC7821_CS = sys_clk;

//DAC输出数据，片选下降沿更改数据，DAC在上升沿输出数据
always @(negedge DAC7821_CS)begin
    if(!sys_rstn)begin
      DAC7821_PIN <= 12'd0;
    end
    else begin
		DAC7821_PIN <= DAC7821_Data;
    end
end


endmodule