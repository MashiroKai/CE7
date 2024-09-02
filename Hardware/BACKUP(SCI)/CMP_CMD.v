
module CMP_CMD(
    input       clk,
    input       rst_n,
    input       valid,
    input       [7:0]rx_data,
    output  reg RECORD, 
    output  reg DAQ
);
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        RECORD <= 1'b0;
        DAQ    <= 1'b0;      
    end
    else if(valid)begin
        case (rx_data)
        8'h0F    :begin
        RECORD <= 1'b0;
        DAQ    <= 1'b1; 
        end 
        8'hF0    :begin
        RECORD <= 1'b1;
        DAQ    <= 1'b0;    
        end 
        8'hFF    :begin
        RECORD <= 1'b1;
        DAQ    <= 1'b1;     
        end 
        default :begin
        RECORD <= 1'b0;
        DAQ    <= 1'b0; 
        end
        endcase
    end
    else begin
        RECORD <= 1'b0;
        DAQ    <= 1'b0; 
    end
end
endmodule