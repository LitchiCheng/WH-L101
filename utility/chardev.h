#ifndef CHARDEV_H
#define CHARDEV_H
#include <stdint.h>
#include <Timer.h>
#include <string.h>
class CCharDev
{
	public:
		virtual int open() = 0;
		virtual int close() = 0;
	
		virtual int write(const uint8_t*, uint32_t) = 0;
		virtual int read(uint8_t*, uint32_t) = 0;
	
		virtual uint32_t data_in_write_buf() = 0;
		virtual uint32_t freesize_in_write_buf() = 0;
		virtual uint32_t data_in_read_buf() = 0;
		virtual void clear_read_buf() = 0;
		virtual bool isTransmitterIdle() = 0;
		
		virtual void runTransmitter() = 0;
		virtual void runReceiver() = 0;
		
		CCharDev(uint32_t break_period = 0)
			:prev_rx_data_num_(0),
			flow_break_period_(break_period)
		{
		}
		
	/**
		* @brief  update_data_break_flag
		* @param  None
		* @retval None
		*/
		
		bool is_dataflow_break()
		{
			if(0 == flow_break_period_ || 0 == data_in_read_buf())
				return false;
			
			return (BaseTimer::Instance()->getTime() - prev_rx_data_timestamp_ > flow_break_period_);
		}
		
	/**
		* @brief  update_data_break_flag
		* @param  None
		* @retval None
		*/
		void update_data_break_flag()
		{
			if(0 == flow_break_period_)
				return;
			
			if(data_in_read_buf() > prev_rx_data_num_ || data_in_read_buf() == 0)
				prev_rx_data_timestamp_ = BaseTimer::Instance()->getTime();
			
			prev_rx_data_num_ = data_in_read_buf();
		}
	
		void run()
		{
			runTransmitter();
			runReceiver();
		}
	
	private:
		uint32_t prev_rx_data_num_;
		uint16_t flow_break_period_;
		uint32_t prev_rx_data_timestamp_;
		
};
#endif
//end of file
