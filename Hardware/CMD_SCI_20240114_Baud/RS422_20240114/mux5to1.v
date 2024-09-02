module mux5to1(
        input       clk
        ,input       rst_n
        //tc
        ,input      empty_tc
        ,input      tx_done_tc
        ,input      valid_tc
        ,input      [7:0]din_tc
        ,output     reg ren_tc
        //hk
        ,input      empty_hk
        ,input      tx_done_hk
        ,input      valid_hk
        ,input      [7:0]din_hk
        ,output     reg ren_hk
        //sd
        ,input      empty_sd
        ,input      tx_done_sd
        ,input      valid_sd
        ,input      [7:0]din_sd
        ,output     reg ren_sd
        //di
        ,input      empty_di
        ,input      tx_done_di
        ,input      valid_di
        ,input      [7:0]din_di
        ,output     reg ren_di
        //pf
        ,input      empty_pf
        ,input      tx_done_pf
        ,input      valid_pf
        ,input      [7:0]din_pf
        ,output     reg ren_pf
        //connect to cmdtxctrl
        ,output     reg empty
        ,output     reg tx_done
        ,input      ren
        ,output     reg valid
        ,output     reg [7:0]dout
);
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
always @(*) begin
    if (!rst_n) begin
        empty = 1'b0;
        tx_done = 1'b0;
        valid = 1'b0;
        dout = 8'd0;
        ren_di = 1'b0;
        ren_hk = 1'b0;
        ren_pf = 1'b0;
        ren_sd = 1'b0;
        ren_tc = 1'b0;
    end
    else begin
        case (STATE)
        ST0    : begin
                empty = empty_pf;
                tx_done = tx_done_pf;
                valid = valid_pf;
                dout = din_pf;
                ren_pf =ren;
                ren_di = 1'b0;
                ren_hk = 1'b0;
                ren_sd = 1'b0;
                ren_tc = 1'b0;
        end
        ST1    :begin
                empty = empty_di;
                tx_done = tx_done_di;
                valid = valid_di;
                dout = din_di;
                ren_pf = 1'b0;
                ren_di = ren;
                ren_hk = 1'b0;
                ren_sd = 1'b0;
                ren_tc = 1'b0;
        end     
        ST2    :begin
                empty = empty_sd;
                tx_done = tx_done_sd;
                valid = valid_sd;
                dout = din_sd;
                ren_pf = 1'b0;
                ren_di = 1'b0;
                ren_hk = 1'b0;
                ren_sd = ren;
                ren_tc = 1'b0;
        end 
        ST3    :begin
                empty = empty_hk;
                tx_done = tx_done_hk;
                valid = valid_hk;
                dout = din_hk;
                ren_pf = 1'b0;
                ren_di = 1'b0;
                ren_hk = ren;
                ren_sd = 1'b0;
                ren_tc = 1'b0;
        end 
        ST4    :begin
                empty = empty_tc;
                tx_done = tx_done_tc;
                valid = valid_tc;
                dout = din_tc;
                ren_pf = 1'b0;
                ren_di = 1'b0;
                ren_hk = 1'b0;
                ren_sd = 1'b0;
                ren_tc = ren;
        end 
            default:begin
                empty = 1'b0;
                tx_done = 1'b0;
                valid = 1'b0;
                dout = 8'd0;
                ren_pf = 1'b0;
                ren_di = 1'b0;
                ren_hk = 1'b0;
                ren_sd = 1'b0;
                ren_tc = 1'b0;
            end
        endcase
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST0;
    end
    else begin
        case (STATE)
        ST0    :begin
                if (!empty_pf) begin
                    STATE <= ST1;
                end
                else begin
                    STATE <= ST0;
                end
        end
        ST1    :begin
                if (!empty_di) begin
                    STATE <= ST2;
                end
                else begin
                    STATE <= ST1;
                end
        end 
        ST2     :begin
                if (!empty_sd) begin
                    STATE <= ST3;
                end
                else begin
                    STATE <= ST2;
                end
        end
        ST3     :begin
                if (!empty_hk) begin
                    STATE <= ST4;
                end
                else begin
                    STATE <= ST3;
                end
        end
        ST4     :begin
                if (!empty_tc) begin
                    STATE <= ST0;
                end
                else begin
                    STATE <= ST4;
                end
        end 
            default: STATE <= ST0;
        endcase
    end
end
endmodule