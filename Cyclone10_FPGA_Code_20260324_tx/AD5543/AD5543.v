module AD5543(
	// 输入
	input          sys_clk,
	input          sys_rstn,
	input          start,
	input   [11:0] dac_Data,
	input	 [3:0] Amp,

	// AD5543接口
	output  reg    AD5543_CLK,
	output  reg    AD5543_SDI,
	output  reg    AD5543_CS
);

wire [15:0] AD5543_Data;
DA_Mul_IP_16Bit	DA_Mul_IP_u (
	.dataa ( dac_Data ),
	.datab ( Amp ),
	.result ( AD5543_Data )
	);

reg [4:0] bit_cnt;
reg [15:0] shift_reg;
reg [2:0] state;
reg [4:0] wait_cnt;   // 最后等待计数器，满足一共50个CLK

//DAC State
localparam  IDLE      = 3'd0,
			LOAD      = 3'd1,
			SEND_0    = 3'd2,
			SEND_1    = 3'd3,	
			DONE_0    = 3'd4,
			DONE_1    = 3'd5;
//状态机，共1+1+32+1+1+14=50个CLK,驱动时钟50M数据刷新 1MHz。
always @(posedge sys_clk or negedge sys_rstn) begin
	if (!sys_rstn) begin
		AD5543_CLK <= 1'b0;
		AD5543_SDI <= 1'b0;
		AD5543_CS  <= 1'b1;
		bit_cnt    <= 5'd0;
		shift_reg  <= 16'd0;
		wait_cnt <= 5'd0;
		state      <= IDLE;
	end else begin
		case (state)
			IDLE: begin									//1个周期
				AD5543_CS  <= 1'b1;
				AD5543_CLK <= 1'b0;
				if (start) begin
					state     <= LOAD;
				end
			end

			LOAD: begin									//1个周期
				AD5543_CS  <= 1'b0;
				shift_reg  <= AD5543_Data;
				bit_cnt    <= 5'd15;
				state      <= SEND_0;
			end
			// 发送数据
			// 在下降沿发送数据，时钟上升沿采样
			SEND_0: begin								//32个周期
				AD5543_CLK <= 1'b0;
				AD5543_SDI <= shift_reg[bit_cnt];
				state      <= SEND_1;
			end
			SEND_1: begin
				AD5543_CLK <= 1'b1;
				if (bit_cnt == 0) begin
					state <= DONE_0;
				end else begin
					bit_cnt <= bit_cnt - 1'b1;
					state   <= SEND_0;
				end
			end
			// 发送完毕后，片选信号拉高, tCSH > 10ns (时钟至CS保持)
			DONE_0: begin									//1个周期
				AD5543_CLK <= 1'b0;
				AD5543_SDI <= 1'b0; // 清除数据线
				wait_cnt    <= 5'd14;          // 装载等待 14 个周期
				state      <= DONE_1;
			end
			DONE_1: begin									//1+14个周期
				AD5543_CS   <= 1'b1;
				if (wait_cnt == 0) begin	
				  state <= IDLE;
				end else begin
				  wait_cnt <= wait_cnt - 1'b1;
				  // state 保持在 DONE_1
				end
			end

			default: state <= IDLE;
		endcase
	end
end

endmodule