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
DA_Mul_IP	DA_Mul_IP_inst (
	.dataa ( ROM_Data ),
	.datab ( Amp ),
	.result ( DAC7821_Data )
	);

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