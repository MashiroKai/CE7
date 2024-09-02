module rx_stop(
    input                   clk ,//80mhz clk
    input                  rst_n, 
    input                   LVDS,//20mhz bits stream 
    output          reg[9:0]dout,
    output          reg     valid
);
//if it comes a comma,then agin the clk

//synchronization 
reg lvds_reg0,lvds_reg1,lvds_reg2,lvds_reg3;
reg               valid_1bit;
reg[29:0]data_30bits;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        lvds_reg0 <= 1'b0;
        lvds_reg1 <= 1'b0;
    end else begin
        lvds_reg0 <= LVDS;
        lvds_reg1 <= lvds_reg0;
        lvds_reg2 <= lvds_reg1;
        lvds_reg3 <= lvds_reg2;
    end
end
//4bits shift register
reg [5:0]shift;
wire asyn_c3;
wire [3:0]cnt;
wire pulse_10;
reg asyn_c10;

assign asyn_c3 = lvds_reg2^lvds_reg3;

counter #(
    .RST(3),
    .START(0)
)
c3(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(asyn_c3)
    ,.en(1'b1)
    ,.cnt(cnt)
);
counter #(
    .RST(10),
    .START(0)
)
c10(
    .clk(clk),//80mhz
    .rst_n(rst_n),
    .asyn(asyn_c10),
    .en(valid_1bit),
    .pulse(pulse_10)//20mhz cnt
);
//10bits shift register
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        data_30bits <= 30'b0;
        valid_1bit       <= 1'b0;
    end
    else begin
        if (valid_1bit) begin
            valid_1bit <= 1'b0;
        end
        else if (cnt == 3'd1) begin
            data_30bits <= {lvds_reg2,data_30bits[29:1]};
            valid_1bit       <= 1'b1;
        end
    end
end

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        asyn_c10 <= 1'b0;
    end
    else begin
        if (asyn_c10) begin
            asyn_c10 <= 1'b0;
        end
        else if (valid_1bit&&(data_30bits == 30'b101000001101011111001010000011)) begin
            asyn_c10 <= 1'b1;
    end
    end 
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        valid <= 1'b0;
        dout <= 10'b0;
    end
    else begin
        if (pulse_10) begin
            //if (data_10bits != 10'b1010000011) begin
                //if (data_10bits != 10'b0101111100) begin
                dout  <= data_30bits[29:20];
                valid <= 1'b1;                     
                end
       // end
       // end
        else begin
                valid <= 1'b0;
        end
    end
end
endmodule