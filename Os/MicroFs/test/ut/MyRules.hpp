

    // ------------------------------------------------------------------------------------------------------
    // Rule:  InitFileSystem
    //
    // ------------------------------------------------------------------------------------------------------
    struct InitFileSystem : public STest::Rule<Os::Tester> {

            // ----------------------------------------------------------------------
            // Construction
            // ----------------------------------------------------------------------

            //! Constructor
            InitFileSystem(U32 numBins, U32 fileSize, U32 numFiles);

            // ----------------------------------------------------------------------
            // Public member functions
            // ----------------------------------------------------------------------

            //! Precondition
            bool precondition(
                const Os::Tester& state //!< The test state
            );

            //! Action
            void action(
                Os::Tester& state //!< The test state
            );

            U32 numBins;
            U32 fileSize;
            U32 numFiles;


    };

    



    // ------------------------------------------------------------------------------------------------------
    // Rule:  OpenFile
    //
    // ------------------------------------------------------------------------------------------------------
    struct OpenFile : public STest::Rule<Os::Tester> {

            // ----------------------------------------------------------------------
            // Construction
            // ----------------------------------------------------------------------

            //! Constructor
            OpenFile();

            // ----------------------------------------------------------------------
            // Public member functions
            // ----------------------------------------------------------------------

            //! Precondition
            bool precondition(
                const Os::Tester& state //!< The test state
            );

            //! Action
            void action(
                Os::Tester& state //!< The test state
            );


    };

    



    // ------------------------------------------------------------------------------------------------------
    // Rule:  Cleanup
    //
    // ------------------------------------------------------------------------------------------------------
    struct Cleanup : public STest::Rule<Os::Tester> {

            // ----------------------------------------------------------------------
            // Construction
            // ----------------------------------------------------------------------

            //! Constructor
            Cleanup();

            // ----------------------------------------------------------------------
            // Public member functions
            // ----------------------------------------------------------------------

            //! Precondition
            bool precondition(
                const Os::Tester& state //!< The test state
            );

            //! Action
            void action(
                Os::Tester& state //!< The test state
            );

    };

    



    // ------------------------------------------------------------------------------------------------------
    // Rule:  WriteData
    //
    // ------------------------------------------------------------------------------------------------------
    struct WriteData : public STest::Rule<Os::Tester> {

            // ----------------------------------------------------------------------
            // Construction
            // ----------------------------------------------------------------------

            //! Constructor
            WriteData();

            // ----------------------------------------------------------------------
            // Public member functions
            // ----------------------------------------------------------------------

            //! Precondition
            bool precondition(
                const Os::Tester& state //!< The test state
            );

            //! Action
            void action(
                Os::Tester& state //!< The test state
            );

    };

    