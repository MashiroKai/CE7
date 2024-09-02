module upload(
        input   clk // 80mhz
        ,input  rst_n
        ,input  empty
        ,input  TXE_N
        ,output  reg rd_en
        ,output  reg valid
);
reg [4:0]STATE;
localparam ST0= 5'b00001;
localparam ST1 = 5'b00010;
localparam ST2 = 5'b00100;
localparam ST3 = 5'b01000;
localparam ST4 = 5'b10000;
reg TXE_N_r;
always @(posedge clk) begin
    TXE_N_r <= TXE_N;
end
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            STATE <= ST0;
        end
        else begin
            case (STATE)
            ST0    :
            begin
                valid <= 1'b1;
                if (!TXE_N_r && !empty&&!TXE_N) begin
                    STATE <= ST1;
                    rd_en <= 1'b1;
                end
                else begin
                    STATE <= ST0;
                    rd_en <= 1'b0;
                end
            end 
            ST1    :
            begin
                valid <= 1'b1;
                rd_en <= 1'b0;
                STATE <= ST2;
            end
            ST2    :
            begin
                valid <= 1'b0;
                rd_en <= 1'b0;
                STATE <= ST3;
            end
            ST3    :
            begin
                valid <= 1'b0;
                rd_en <= 1'b0;
                STATE <= ST4;
            end
            ST4    :
            begin
                valid <= 1'b0;
                rd_en <= 1'b0;
                STATE <= ST0;
            end
            endcase
        end
end
endmodule