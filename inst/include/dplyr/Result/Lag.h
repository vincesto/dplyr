#ifndef dplyr_Result_Lag_H
#define dplyr_Result_Lag_H

namespace dplyr {

    template <int RTYPE>
    class Lag : public Result {
    public:
        typedef typename scalar_type<RTYPE>::type STORAGE ;

        Lag( SEXP data_, int n_, const RObject& def_) :
            data(data_),
            n(n_),
            def( Vector<RTYPE>::get_na() )
        {
          if( !Rf_isNull(def_) ){
            def = as<STORAGE>(def_) ;
          }
        }

        virtual SEXP process(const GroupedDataFrame& gdf ){
            int nrows = gdf.nrows() ;
            int ng = gdf.ngroups() ;

            Vector<RTYPE> out = no_init(nrows) ;
            GroupedDataFrame::group_iterator git = gdf.group_begin();
            for( int i=0; i<ng; i++, ++git){
                process_slice(out, *git, *git) ;
            }
            copy_most_attributes( out, data ) ;
            return out ;
        }

        virtual SEXP process(const RowwiseDataFrame& gdf ){
            Vector<RTYPE> out(gdf.nrows(), def ) ;
            copy_most_attributes( out, data ) ;
            return out ;
        }

        virtual SEXP process(const FullDataFrame& df){
            int nrows = df.nrows() ;
            Vector<RTYPE> out = no_init(nrows) ;
            SlicingIndex index = df.get_index() ;
            process_slice( out, index, index );
            copy_most_attributes( out, data ) ;
            return out ;
        }

        virtual SEXP process(const SlicingIndex& index){
            int nrows = index.size() ;
            Vector<RTYPE> out = no_init(nrows) ;
            SlicingIndex fake(0, nrows) ;
            process_slice( out, index, fake );
            copy_most_attributes( out, data ) ;
            return out ;
        }

    private:

        void process_slice( Vector<RTYPE>& out, const SlicingIndex& index, const SlicingIndex& out_index){
            int chunk_size = index.size() ;
            int i=0 ;

            if( n > chunk_size ) {
                for(int i=0; i<chunk_size ; i++){
                    out[out_index[i]] = def ;
                }
            } else {
                for(; i<n ; i++){
                    out[out_index[i]] = def ;
                }
                for( ; i<chunk_size; i++ ){
                    out[out_index[i]] = data[index[i-n]] ;
                }
            }
        }

        Vector<RTYPE> data ;
        int n ;
        STORAGE def ;
    } ;

}

#endif
