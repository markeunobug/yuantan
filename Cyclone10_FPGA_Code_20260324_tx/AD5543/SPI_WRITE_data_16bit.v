module SPI_WRITE_data_16bit(
	input rst,
	input clk,
	input spi_tx_en,
	input [15:0] tx_data,
	
	output reg spi_tx_done,
	output reg spi_cs,
	output reg spi_sclk,	
	output reg spi_din
);

(* preserve *)reg [7:0] tx_state;/*synthesis noprune*/
	
wire 		spi_tx_en_pos;
reg 		spi_tx_en_d0;
reg 		spi_tx_en_d1;
assign   spi_tx_en_pos = spi_tx_en_d0 & (~spi_tx_en_d1);  

always @(posedge clk or negedge rst)
begin 
		if(!rst)
			begin
				spi_tx_en_d0 <= 1'b0;
				spi_tx_en_d1 <= 1'b0; 
			end
		else
			begin
				spi_tx_en_d0  <= spi_tx_en;                   
				spi_tx_en_d1  <= spi_tx_en_d0;
			end
end

always @(posedge clk)
begin
	if(spi_tx_en_pos) begin
		spi_cs <= 1'b0;
		spi_sclk <= 1'b0;
		spi_din <= 1'b0;
		tx_state <= 8'd0;
		spi_tx_done <= 1'b0;
	end
	else begin
		case(tx_state)//spi的速率取决于clk/2
		8'd0: begin
			spi_tx_done <= 1'b0;
			spi_sclk <= 1'b0;
			spi_din <= tx_data[15];		//MSB first	
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd1: begin
			spi_sclk <= 1'b1;			//上升沿将数据写入
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd2: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[14];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd3: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd4: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[13];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd5: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd6: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[12];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd7: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd8: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[11];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd9: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd10: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[10];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd11: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd12: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[9];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd13: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd14: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[8];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd15: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd16: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[7];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd17: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd18: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[6];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd19: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd20: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[5];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd21: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd22: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[4];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd23: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd24: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[3];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd25: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd26: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[2];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd27: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd28: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[1];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd29: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd30: begin
			spi_sclk <= 1'b0;
			spi_din <= tx_data[0];
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd31: begin
			spi_sclk <= 1'b1;
			tx_state <= tx_state + 1'b1 ; 
		end
		8'd32: begin
			spi_sclk <= 1'b0;
			spi_tx_done <= 1'b1;	//传输完成
			tx_state <= tx_state;	//保持状态。有缺陷，状态会一直卡在这里，直到下一次 spi_tx_en_pos 来临
			spi_cs <= 1'b1;			//释放总线
		end
		default : tx_state <= tx_state;
		endcase
	end
end

endmodule
