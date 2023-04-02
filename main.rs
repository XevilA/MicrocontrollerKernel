use serialport::{SerialPortSettings, DataBits, FlowControl, Parity, StopBits};
use std::io::{self, Write};
use std::time::Duration;

fn main() -> io::Result<()> {
    // Set up serial port settings
    let mut settings = SerialPortSettings::default();
    settings.baud_rate = 9600;
    settings.data_bits = DataBits::Eight;
    settings.flow_control = FlowControl::None;
    settings.parity = Parity::None;
    settings.stop_bits = StopBits::One;

    // Connect to serial port
    let port = serialport::new("/dev/ttyUSB0", &settings).open()?;
    
    // Send command to board
    let command = "digitalWrite(13, HIGH)\n";
    port.write_all(command.as_bytes())?;
    
    // Wait for board to process command
    std::thread::sleep(Duration::from_millis(100));
    
    // Send another command to board
    let command = "digitalWrite(13, LOW)\n";
    port.write_all(command.as_bytes())?;
    
    // Wait for board to process command
    std::thread::sleep(Duration::from_millis(100));
    
    // Close serial port
    drop(port);
    
    Ok(())
}
