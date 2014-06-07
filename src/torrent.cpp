#include "torrent.h"

Torrent::Torrent()
{
    s.listen_on(std::make_pair(6881, 6889), ec);

    /*
    if (ec)
    {
        fprintf(stderr, "failed to open listen socket: %s\n", ec.message().c_str());
        return 1;
    }
    */
}

Torrent::~Torrent()
{
}

void Torrent::download()//(QString tmpPath, QString fileTorrent)
{   
    params.save_path = "/tmp/";//tmpPath;
    params.ti = new torrent_info("AE6D1A22C6E7DD5490A8F21E9A57D78E691E389C.torrent", ec);//(fileTorrent, ec);
    /*
    p.ti = new torrent_info(argv[1], ec);
    if (ec)
    {
        fprintf(stderr, "%s\n", ec.message().c_str());
        return 1;
    }
    */

    handle = s.add_torrent(params, ec);

    /*
    if (ec)
    {
        fprintf(stderr, "%s\n", ec.message().c_str());
        return 1;
    }
    */

    handle.set_sequential_download(true); // Descarga sequencial.

    /* 
    handle.file_priority(0, 7); // Prioridad de fila maxima, solo descarga esta fila.
    handle.file_priority(1, 0); // Prioridad de fila cero, no descarga esta fila.
    libtorrent::torrent_info torrent_infox("AE6D1A22C6E7DD5490A8F21E9A57D78E691E389C.torrent"); // Numero de piezas.
    std::cout << torrent_infox.num_pieces(); // 782

    for (int i=0; i<783; i++) { // Prioridad de pieza. Que parte del torrent(video) descarga.
        if (i<200)
          handle.piece_priority(i, 7);
        else if (i>200 && i<650)
          handle.piece_priority(i, 0);
        else
          handle.piece_priority(i, 7);
    }
    */
}

