// Dear emacs, this is -*- c++ -*-
// Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#ifndef MEMORY_CUH
#define MEMORY_CUH

// Local include(s).
#include "Macros.cuh"

// System include(s).
#include <memory>
#include "cuda.h"

   /// Namespace for code the user should not be relying on directly
   namespace details {

      /// Class performing the deletion of a CUDA managed memory array
      class ArrayDeleter {

      public:
         /// Operator performing the deletion of the memory
         void operator()( void* ptr );

      }; // class ArrayDeleter

      /// Class performing the deletion of a CUDA device memory array
      class DeviceArrayDeleter {

      public:
         /// Operator performing the deletion of the memory
         void operator()( void* ptr );

      }; // class DeviceArrayDeleter

      /// Class performing the deletion of pinned host memory
      class HostArrayDeleter {

      public:
         /// Operator performing the deletion of the memory
         void operator()( void* ptr );

      }; // class HostArrayDeleter

   } // namespace details

   /// Convenience type for handling primitive variable arrays with CUDA
   template< typename T >
   using unique_array = std::unique_ptr< T, details::ArrayDeleter >;

   /// Function creating a primitive array in CUDA managed memory
   template< typename T >
   unique_array< T > make_unique_array( std::size_t size );

   /// Convenience type for using primitive variable arrays on a CUDA device
   template< typename T >
   using device_array = std::unique_ptr< T, details::DeviceArrayDeleter >;

   /// Function creating a primitive array in CUDA device memory
   template< typename T >
   device_array< T > make_device_array( std::size_t size );

   /// Convenience type for using primitive variable arrays on the host
   template< typename T >
   using host_array = std::unique_ptr< T, details::HostArrayDeleter >;

   /// Function creating a primitive array in the host's memory
   template< typename T >
   host_array< T > make_host_array( std::size_t size );

   /// Replacement for @c std::vector in device code
   ///
   /// Since STL classes can't be used in CUDA device code, some helper
   /// code is needed to manage variable sized arrays in some convenient
   /// way.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   template< typename T >
   class array {

   public:
      /// Default constructor
      HOST_AND_DEVICE array();
      /// Constructor allocating some memory right away
      HOST_AND_DEVICE
      array( std::size_t size );
      /// Move constructor
      HOST_AND_DEVICE
      array( array< T >&& parent );
      /// Destructor
      HOST_AND_DEVICE
      ~array();

      /// Move operator
      HOST_AND_DEVICE
      array< T >& operator=( array< T >&& rhs );

      /// Get the size of the array
      HOST_AND_DEVICE
      std::size_t size() const;

      /// Get the pointer to the array held by the object (non-const)
      HOST_AND_DEVICE
      T* get();
      /// Get the pointer to the array held by the object (const)
      HOST_AND_DEVICE
      const T* get() const;

      /// Conversion operator to a bare pointer (non-const)
      HOST_AND_DEVICE
      operator T*() { return get(); }
      /// Conversion operator to a bare pointer (const)
      HOST_AND_DEVICE
      operator const T*() const { return get(); }

      /// Operator for accessing one element of the array (non-const)
      HOST_AND_DEVICE
      T& operator[]( std::size_t index );
      /// Operator for accessing one element of the array (const)
      HOST_AND_DEVICE
      const T& operator[]( std::size_t index ) const;

      /// Resize the managed array
      HOST_AND_DEVICE
      bool resize( std::size_t size );

   private:
      /// Pointer to the managed array
      T* m_ptr;
      /// Size of the managed array
      std::size_t m_size;

   }; // class array


// Include the implementation.
#include "Memory.icc"

#endif // ATHCUDACORE_MEMORY_CUH
