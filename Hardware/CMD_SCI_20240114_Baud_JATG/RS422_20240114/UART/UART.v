/*
UART #(
	.CLKFREQ(12000000)
	,.MASTER_BAUD(115200)
	,.SLAVE_BAUD(115200)
)uut_UART(
        //SYSTEM INTERFACE
			.clk(),
			.rst_n(),
        //MASTER INTERFACE
			.master_rx(),
			.master_tx(),
            //SEND DATA PORT
			.master_send(),
			.master_tx_data(),//8bits
			.master_ready(),
            //RECEIVE DATA PORT
			.master_valid(),
			.master_rx_data(),//8bits
			.master_cbe(),//check bit error   
			.master_sbe(),//stop bit error
        //SLAVE INTERFACE
			.slave_rx(),
			.slave_tx(),
            //SEND DATA PROT
			.slave_send(),
			.slave_tx_data(),
			.slave_ready(),
            //RECEIVE DATA PORT
			.slave_valid(),
			.slave_rx_data(),
			.slave_cbe(),//check bit error
			.slave_sbe()//stop bit error

);
*/
//can config baud
module UART #(
        parameter       MASTER_BAUD  = 921600,
        parameter       CLKFREQ  = 12000000
)
(
        //SYSTEM INTERFACE
        input           clk,
        input           rst_n,
		//UART CONFIG INTERFACE
		input			parity,
		input			[1:0]stopbit,
		input			[31:0]SLAVE_BAUD,
        //MASTER INTERFACE
        input           master_rx,
        output          master_tx,
            //SEND DATA PORT
        input           master_send,
        input   [7:0]   master_tx_data,
        output          master_ready,
            //RECEIVE DATA PORT
        output          master_valid,
        output  [7:0]   master_rx_data,
        output          master_cbe,//check bit error
        output          master_sbe,//stop bit error
        //SLAVE INTERFACE
        input           slave_rx,
        output          slave_tx,
            //SEND DATA PROT
        input           slave_send,
        input   [7:0]   slave_tx_data,
        output          slave_ready,
            //RECEIVE DATA PORT
        output          slave_valid,
        output  [7:0]   slave_rx_data,
        output          slave_cbe,//check bit error
        output          slave_sbe//stop bit error
);
localparam MASTER_BPS_PARA = CLKFREQ/MASTER_BAUD;
wire [31:0]SLAVE_BPS_PARA_TX = CLKFREQ/SLAVE_BAUD;
localparam SLAVE_BPS_PARA_RX = CLKFREQ/115200;

//MASTER UART MODULE
Baud_rx #(
	.BPS_PARA(MASTER_BPS_PARA)
)
m_baud_rx(
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(m_rx_bps_en),
	.bps_clk(m_rx_bps_clk)
);
wire m_rx_bps_en;
wire m_rx_bps_clk;

Uart_Rx m_rx
(
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(m_rx_bps_en),
	.bps_clk(m_rx_bps_clk),
	.rs422_rx(master_rx),
	.rx_data(master_rx_data),
	.valid(master_valid),
	.check(master_cbe),
	.stop(master_sbe)
);

Baud_tx #(
	.BPS_PARA(MASTER_BPS_PARA)
)
m_baud_tx(
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(m_tx_bps_en),
	.bps_clk(m_tx_bps_clk)
);
wire m_tx_bps_en;
wire m_tx_bps_clk;

Uart_Tx m_tx(
	.clk(clk),
	.rst_n(rst_n),
	.parity(0),
	.stopbit(2'b11),
	.bps_en(m_tx_bps_en),
	.bps_clk(m_tx_bps_clk),
	.valid(master_send),
	.tx_data(master_tx_data),
	.rs422_tx(master_tx),
	.ready(master_ready)
);



//SLAVE UART MODULE
Baud_rx #(
	.BPS_PARA(SLAVE_BPS_PARA_RX)
)
s_baud_rx(
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(s_rx_bps_en),
	.bps_clk(s_rx_bps_clk)
);
wire s_rx_bps_en;
wire s_rx_bps_clk;

Uart_Rx s_rx
(
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(s_rx_bps_en),
	.bps_clk(s_rx_bps_clk),
	.rs422_rx(slave_rx),
	.rx_data(slave_rx_data),
	.valid(slave_valid),
	.check(slave_cbe),
	.stop(slave_sbe)
);

Baud_tx_slave s_baud_tx(
	.BPS_PARA(SLAVE_BPS_PARA_TX),
	.clk(clk),
	.rst_n(rst_n),
	.bps_en(s_tx_bps_en),
	.bps_clk(s_tx_bps_clk)
);
wire s_tx_bps_en;
wire s_tx_bps_clk;

Uart_Tx s_tx(
	.clk(clk),
	.rst_n(rst_n),
	.parity(parity),
	.stopbit(stopbit),
	.bps_en(s_tx_bps_en),
	.bps_clk(s_tx_bps_clk),
	.valid(slave_send),
	.tx_data(slave_tx_data),
	.rs422_tx(slave_tx),
	.ready(slave_ready)
);

endmodule