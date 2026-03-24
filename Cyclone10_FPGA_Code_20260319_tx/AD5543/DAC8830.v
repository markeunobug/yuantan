module DAC8830(
    input clk,
    input rst,
	 input spi_drive_clk,
	 
    input [15:0] output_data,
	 output spi_cs   	,
	 output spi_sclk	,
	 output spi_din	
);


reg	spi_tx_en;
(* preserve *)reg [1:0] DA_state;/*synthesis noprune,不要优化*/
reg [9:0] delay_count;


//抓取spi_tx_done 上升沿，生成一个单时钟周期脉冲 spi_tx_done_pos
wire 		spi_tx_done;
wire 		spi_tx_done_pos;
reg		spi_tx_done_d0;
reg		spi_tx_done_d1;
assign   spi_tx_done_pos = spi_tx_done_d0 & (~spi_tx_done_d1);
always @(posedge clk or negedge rst)
begin 
		if(!rst)
			begin
				spi_tx_done_d0 <= 1'b0;
				spi_tx_done_d1 <= 1'b0; 
			end
		else
			begin
				spi_tx_done_d0  <= spi_tx_done;          
				spi_tx_done_d1  <= spi_tx_done_d0;
			end
end
//状态机轮询
always @(posedge clk or negedge rst)
begin
	if(!rst) begin
		DA_state <= 2'd0;
		spi_tx_en <= 1'b0;
		delay_count <= 10'd0;
	end
	else begin
		case(DA_state)
		//状态一，开始
		2'd0: begin
			delay_count <= 10'd0;
			spi_tx_en <= 1'b1;
			DA_state <= DA_state + 1'd1;
		end
		//状态二，发送完成
		2'd1: begin
			delay_count <= delay_count + 10'd1;
			if(spi_tx_done_pos) begin
				DA_state <= DA_state + 1'd1;
				spi_tx_en <= 1'b0;
			end
		end
		//状态三，延时
		2'd2: begin
				delay_count <= delay_count + 10'd1;
				DA_state <= DA_state;
				if(delay_count >= 10'd100) //延时100个时钟周期
					DA_state <= 2'd0;		//状态机回到状态一
		end
		default :DA_state <= DA_state;//防止状态机出错
		endcase
	end
end

SPI_WRITE_data_16bit SPI_Write_data_16bit_u(
	.clk			( spi_drive_clk ),
	.rst			( rst ),
	.spi_tx_en		( spi_tx_en ),
	.tx_data 		( output_data ),
	
	.spi_tx_done	( spi_tx_done ),
	.spi_cs			( spi_cs ),
	.spi_sclk		( spi_sclk ),	
	.spi_din		( spi_din ) 
);

endmodule 