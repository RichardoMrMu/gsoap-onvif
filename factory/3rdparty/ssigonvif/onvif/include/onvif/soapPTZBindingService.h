/* soapPTZBindingService.h
   Generated by gSOAP 2.8.17r from onvif.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapPTZBindingService_H
#define soapPTZBindingService_H
#include "soapH.h"
class SOAP_CMAC PTZBindingService
{ public:
	struct soap *soap;
	bool own;
	/// Constructor
	PTZBindingService();
	/// Constructor to use/share an engine state
	PTZBindingService(struct soap*);
	/// Constructor with engine input+output mode control
	PTZBindingService(soap_mode iomode);
	/// Constructor with engine input and output mode control
	PTZBindingService(soap_mode imode, soap_mode omode);
	/// Destructor, also frees all deserialized data
	virtual ~PTZBindingService();
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to defaults
	virtual	void reset();
	/// Initializer used by constructor
	virtual	void PTZBindingService_init(soap_mode imode, soap_mode omode);
	/// Create a copy
	virtual	PTZBindingService *copy() SOAP_PURE_VIRTUAL;
	/// Close connection (normally automatic)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Return sender-related fault to sender
	virtual	int soap_senderfault(const char *string, const char *detailXML);
	/// Return sender-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Return receiver-related fault to sender
	virtual	int soap_receiverfault(const char *string, const char *detailXML);
	/// Return receiver-related fault with SOAP 1.2 subcode to sender
	virtual	int soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML);
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, struct _wsse__Security *wsse__Security);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Run simple single-thread iterative service on port until a connection error occurs (returns error code or SOAP_OK), use this->bind_flag = SO_REUSEADDR to rebind for a rerun
	virtual	int run(int port);
	/// Bind service to port (returns master socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET bind(const char *host, int port, int backlog);
	/// Accept next request (returns socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET accept();
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
	/// Then accept SSL handshake, when SSL is used
	virtual	int ssl_accept();
#endif
	/// Serve this request (returns error code or SOAP_OK)
	virtual	int serve();
	/// Used by serve() to dispatch a request (returns error code or SOAP_OK)
	virtual	int dispatch();

	///
	/// Service operations (you should define these):
	/// Note: compile with -DWITH_PURE_VIRTUAL for pure virtual methods
	///

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetConfigurations' (returns error code or SOAP_OK)
	virtual	int GetConfigurations(_tptz__GetConfigurations *tptz__GetConfigurations, _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetPresets' (returns error code or SOAP_OK)
	virtual	int GetPresets(_tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse *tptz__GetPresetsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SetPreset' (returns error code or SOAP_OK)
	virtual	int SetPreset(_tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse *tptz__SetPresetResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'RemovePreset' (returns error code or SOAP_OK)
	virtual	int RemovePreset(_tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse *tptz__RemovePresetResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GotoPreset' (returns error code or SOAP_OK)
	virtual	int GotoPreset(_tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse *tptz__GotoPresetResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
	virtual	int GetStatus(_tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse *tptz__GetStatusResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetConfiguration' (returns error code or SOAP_OK)
	virtual	int GetConfiguration(_tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetNodes' (returns error code or SOAP_OK)
	virtual	int GetNodes(_tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse *tptz__GetNodesResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetNode' (returns error code or SOAP_OK)
	virtual	int GetNode(_tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse *tptz__GetNodeResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SetConfiguration' (returns error code or SOAP_OK)
	virtual	int SetConfiguration(_tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetConfigurationOptions(_tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GotoHomePosition' (returns error code or SOAP_OK)
	virtual	int GotoHomePosition(_tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SetHomePosition' (returns error code or SOAP_OK)
	virtual	int SetHomePosition(_tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'ContinuousMove' (returns error code or SOAP_OK)
	virtual	int ContinuousMove(_tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'RelativeMove' (returns error code or SOAP_OK)
	virtual	int RelativeMove(_tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'SendAuxiliaryCommand' (returns error code or SOAP_OK)
	virtual	int SendAuxiliaryCommand(_tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'AbsoluteMove' (returns error code or SOAP_OK)
	virtual	int AbsoluteMove(_tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'Stop' (returns error code or SOAP_OK)
	virtual	int Stop(_tptz__Stop *tptz__Stop, _tptz__StopResponse *tptz__StopResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetPresetTours' (returns error code or SOAP_OK)
	virtual	int GetPresetTours(_tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetPresetTour' (returns error code or SOAP_OK)
	virtual	int GetPresetTour(_tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetPresetTourOptions' (returns error code or SOAP_OK)
	virtual	int GetPresetTourOptions(_tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'CreatePresetTour' (returns error code or SOAP_OK)
	virtual	int CreatePresetTour(_tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'ModifyPresetTour' (returns error code or SOAP_OK)
	virtual	int ModifyPresetTour(_tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'OperatePresetTour' (returns error code or SOAP_OK)
	virtual	int OperatePresetTour(_tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'RemovePresetTour' (returns error code or SOAP_OK)
	virtual	int RemovePresetTour(_tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse) SOAP_PURE_VIRTUAL;

	/// Web service operation 'GetCompatibleConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleConfigurations(_tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse) SOAP_PURE_VIRTUAL;
};
#endif