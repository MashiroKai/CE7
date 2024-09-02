module cmdlength (
        input       clk
        ,input      rst_n
        ,input      [4:0]cmd// platform , data injection , pre shutdown , timecode ,HK
        ,output     reg [7:0]length
);
localparam ST0 = 5'b00001;
localparam ST1 = 5'b00010;
localparam ST2 = 5'b00100;
localparam ST3 = 5'b01000;
localparam ST4 = 5'b10000;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        length <= 8'd 53;
    end
    else begin
        case (cmd)
        ST0    :begin
                length <= 8'd13;
        end
        ST1     :begin
                length <= 8'd9;
        end
        ST2     :begin
                length <= 8'd9;
        end 
        ST3     :begin
                length <= 8'd25;
        end
        ST4     :begin
                length <= 8'd53;
        end
            default: length <= 8'd53;
        endcase
    end
end

endmodule
