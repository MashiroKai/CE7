/*
cmdtxctrl test(
        .clk(clk)
        ,.rst_n(rst_n)
        ,.empty(empty)
        ,.ready(ready)
        ,.ready(ready_waited)
        ,.ENABLE(ENABLE)
        ,.tx_done(tx_done)
        ,.ren(ren)
        ,.tx_en(tx_en)
);
*/
module cmdtxctrl
(
        input       clk
        ,input      rst_n
        ,input      empty
        ,input      ready
        ,input      ready_waited
        ,input      ENABLE
        ,input      tx_done
        ,output     ren  
        ,output     reg tx_en
);
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
always @(*) begin
            if (STATE ==ST1 || !ready) begin
                tx_en = 1'b1;
            end
            else begin
                tx_en = 1'b0;
            end
end
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            STATE <= ST0;
        end
        else begin
            case (STATE)
            ST0    :begin
                        if (!empty&&ENABLE) begin
                            STATE <= ST1;
                        end
                        else begin
                            STATE <= STATE;
                        end
            end
            ST1    :begin
                        if (tx_done||empty) begin
                            STATE <= ST0;
                        end
                        else begin
                            STATE <= STATE;
                        end
            end 
                default: STATE <= ST0;
            endcase
        end
end
reg ren_r;
always @(*) begin
            if (STATE == ST1 && ready_waited) begin
                ren_r = 1'b1;
            end
            else begin
                ren_r = 1'b0;
            end
end
pulsed_reg #(
    .WIDTH(1)
)
p1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.signal(ren_r)
    ,.pos(ren)
);
endmodule