// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    Amstrad PC1512 mouse port emulation

**********************************************************************/

#include "emu.h"
#include "bus/pc1512/mouse.h"



//**************************************************************************
//  DEVICE DEFINITION
//**************************************************************************

const device_type PC1512_MOUSE_PORT = &device_creator<pc1512_mouse_port_t>;
const device_type PC1512_MOUSE = &device_creator<pc1512_mouse_t>;



//**************************************************************************
//  CARD INTERFACE
//**************************************************************************

//-------------------------------------------------
//  device_pc1512_mouse_port_interface - constructor
//-------------------------------------------------

device_pc1512_mouse_port_interface::device_pc1512_mouse_port_interface(const machine_config &mconfig, device_t &device) :
	device_slot_card_interface(mconfig, device)
{
	m_port = dynamic_cast<pc1512_mouse_port_t *>(device.owner());
}



//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  pc1512_mouse_port_t - constructor
//-------------------------------------------------

pc1512_mouse_port_t::pc1512_mouse_port_t(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock) :
	device_t(mconfig, PC1512_MOUSE_PORT, "Amstrad PC1512 mouse port", tag, owner, clock, "pc1512_mouse_port", __FILE__),
	device_slot_interface(mconfig, *this),
	m_write_x(*this),
	m_write_y(*this),
	m_write_m1(*this),
	m_write_m2(*this),
	m_device(nullptr)
{
}

pc1512_mouse_t::pc1512_mouse_t(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock) :
	device_t(mconfig, PC1512_MOUSE, "Amstrad PC1512 mouse", tag, owner, clock, "pc1512_mouse", __FILE__),
	device_pc1512_mouse_port_interface(mconfig, *this)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void pc1512_mouse_port_t::device_start()
{
	m_device = dynamic_cast<device_pc1512_mouse_port_interface *>(get_card_device());

	// resolve callbacks
	m_write_x.resolve_safe();
	m_write_y.resolve_safe();
	m_write_m1.resolve_safe();
	m_write_m2.resolve_safe();
}

void pc1512_mouse_t::device_start()
{
}


//-------------------------------------------------
//  INPUT_PORTS( mouse )
//-------------------------------------------------

static INPUT_PORTS_START( mouse )
	PORT_START("MOUSEB")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_SPECIAL ) PORT_NAME("Left Mouse Button") PORT_CODE(MOUSECODE_BUTTON1) PORT_CHANGED_MEMBER(DEVICE_SELF, pc1512_mouse_t, mouse_button_1_changed, 0)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_SPECIAL ) PORT_NAME("Right Mouse Button") PORT_CODE(MOUSECODE_BUTTON2) PORT_CHANGED_MEMBER(DEVICE_SELF, pc1512_mouse_t, mouse_button_2_changed, 0)

	PORT_START("MOUSEX")
	PORT_BIT( 0xff, 0x00, IPT_MOUSE_X ) PORT_SENSITIVITY(100) PORT_KEYDELTA(5) PORT_MINMAX(0, 255) PORT_PLAYER(1) PORT_CHANGED_MEMBER(DEVICE_SELF, pc1512_mouse_t, mouse_x_changed, 0)

	PORT_START("MOUSEY")
	PORT_BIT( 0xff, 0x00, IPT_MOUSE_Y ) PORT_SENSITIVITY(100) PORT_KEYDELTA(5) PORT_MINMAX(0, 255) PORT_PLAYER(1) PORT_CHANGED_MEMBER(DEVICE_SELF, pc1512_mouse_t, mouse_y_changed, 0)
INPUT_PORTS_END


//-------------------------------------------------
//  input_ports - device-specific input ports
//-------------------------------------------------

ioport_constructor pc1512_mouse_t::device_input_ports() const
{
	return INPUT_PORTS_NAME( mouse );
}


//-------------------------------------------------
//  SLOT_INTERFACE( pc1512_mouse_port_devices )
//-------------------------------------------------

SLOT_INTERFACE_START( pc1512_mouse_port_devices )
	SLOT_INTERFACE("mouse", PC1512_MOUSE)
SLOT_INTERFACE_END
