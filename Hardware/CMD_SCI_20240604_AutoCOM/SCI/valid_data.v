module valid_data(
        input       clk
        ,input      rst_n
        ,input      k
        ,input      valid_in
        ,input      [7:0]din
        ,output     valid
        ,output     [7:0]dout
);
reg k_r,valid_r;
wire neg_k,pos_k;
assign neg_k = !k&&k_r;//acquire the negegde of k
assign pos_k = k&&!k_r;//acquire the posegde of k
assign dout = din;
assign valid = k_r?1'b0:valid_r;
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
                k_r <= 1'b0;
        end
        else begin
                k_r <= k;
        end
end
reg [1:0]STATE;
localparam WR_FIFO = 2'b01;
localparam STOP = 2'b10;
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
                STATE <= STOP;
        end
        else begin
            case (STATE)
                WR_FIFO:
                begin
                        valid_r <= valid_in;
                    if (pos_k) begin
                        STATE <= STOP;
                        valid_r <= 1'b0;
                    end    
                end 
                STOP:
                begin
                        valid_r <= 1'b0;
                   if (neg_k) begin
                        STATE <= WR_FIFO;
                        valid_r <= valid_in;
                   end     
                end
            endcase
        end
end

endmodule