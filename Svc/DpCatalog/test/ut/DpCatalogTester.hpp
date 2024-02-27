// ======================================================================
// \title  DpCatalogTester.hpp
// \author tcanham
// \brief  hpp file for DpCatalog component test harness implementation class
// ======================================================================

#ifndef Svc_DpCatalogTester_HPP
#define Svc_DpCatalogTester_HPP

#include "Svc/DpCatalog/DpCatalogGTestBase.hpp"
#include "Svc/DpCatalog/DpCatalog.hpp"

namespace Svc {

    class DpCatalogTester :
        public DpCatalogGTestBase
    {

    public:

        // ----------------------------------------------------------------------
        // Constants
        // ----------------------------------------------------------------------

        // Maximum size of histories storing events, telemetry, and port outputs
        static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;

        // Instance ID supplied to the component instance under test
        static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

        // Queue depth supplied to the component instance under test
        static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

    public:

        // ----------------------------------------------------------------------
        // Construction and destruction
        // ----------------------------------------------------------------------

        //! Construct object DpCatalogTester
        DpCatalogTester();

        //! Destroy object DpCatalogTester
        ~DpCatalogTester();

    public:

        // ----------------------------------------------------------------------
        // Tests
        // ----------------------------------------------------------------------

        //! To do
        void doInit();

    private:

        // ----------------------------------------------------------------------
        // Handlers for typed from ports
        // ----------------------------------------------------------------------

        //! Handler implementation for fileOut
        Svc::SendFileResponse from_fileOut_handler(
            NATIVE_INT_TYPE portNum, //!< The port number
            const Svc::SendFileRequestPortStrings::StringSize100& sourceFileName, //!< Path of file to downlink
            const Svc::SendFileRequestPortStrings::StringSize100& destFileName, //!< Path to store downlinked file at
            U32 offset, //!< Amount of data in bytes to downlink from file. 0 to read until end of file
            U32 length //!< Amount of data in bytes to downlink from file. 0 to read until end of file
        );

        //! Handler implementation for pingOut
        void from_pingOut_handler(
            NATIVE_INT_TYPE portNum, //!< The port number
            U32 key //!< Value to return to pinger
        );

    private:

        // ----------------------------------------------------------------------
        // Helper functions
        // ----------------------------------------------------------------------

        //! Connect ports
        void connectPorts();

        //! Initialize components
        void initComponents();

    private:

        // ----------------------------------------------------------------------
        // Member variables
        // ----------------------------------------------------------------------

        //! The component under test
        DpCatalog component;

    };

}

#endif
