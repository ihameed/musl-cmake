{-# LANGUAGE NoMonomorphismRestriction, ApplicativeDo, RecordWildCards,
    NamedFieldPuns #-}
module Main (main) where

import Control.Monad (when)
import Data.Foldable (for_)
import Data.Maybe (catMaybes)
import Language.C.Data.Ident (Ident (Ident))
import System.Environment (getArgs)

import qualified Data.List as List
import qualified Language.C as C
import qualified Language.C.System.GCC as Gcc
import qualified Options.Applicative as Opt

collect_decls extern_decls = catMaybes . (flip map) extern_decls $ \extern_decl -> case extern_decl of
    C.CDeclExt decl -> case decl of
        C.CDecl specifiers decl _ -> Just (specifiers, decl)
        C.CStaticAssert _ _ _ -> Nothing
    C.CFDefExt _ -> Nothing
    C.CAsmExt _ _ -> Nothing

is_extern = any $ \spec -> case spec of
    C.CStorageSpec (C.CExtern _) -> True
    _ -> False

ident_name (Ident name _ _) = name

match_single_func_decl (storage_spec, decls) = case (is_extern storage_spec, decls) of
    (True, [(Just (C.CDeclr (Just name) _ _ _ _), Nothing, Nothing)]) -> Just (ident_name name)
    (_, _) -> Nothing

main :: IO ()
main = do
    CmdArgs { project_root, header_file_path, architecture, debug } <- Opt.execParser cmdargs_parser
    let flags =
            [ "-nostdinc"
            , "-I" ++ project_root ++ "/musl-cmake/src/glibc-compat/include"
            , "-I" ++ project_root ++ "/musl-cmake/src/generated-headers/" ++ architecture
            , "-I" ++ project_root ++ "/arch/" ++ architecture
            , "-I" ++ project_root ++ "/arch/generic"
            , "-I" ++ project_root ++ "/include"
            , "-D_XOPEN_SOURCE=200809"
            , "-D_GNU_SOURCE=1"
            ]
    when debug $ do
        putStrLn $ "using flags: " ++ List.intercalate " " flags
    parsed <- C.parseCFile (Gcc.newGCC "gcc") Nothing flags header_file_path
    C.CTranslUnit extern_decls _ <- case parsed of
        Left err -> error ("main: parse error: " ++ show err)
        Right p -> pure p
    let decls = collect_decls extern_decls
    let extern_func_names = [name | Just name <- map match_single_func_decl decls]
    for_ extern_func_names $ \name -> do
        putStrLn $ show name ++ ";"

data CmdArgs = CmdArgs
    { project_root :: String
    , header_file_path :: String
    , architecture :: String
    , debug :: Bool
    }

cmdargs_parser = Opt.info
    (parser Opt.<**> Opt.helper)
    (Opt.fullDesc <> Opt.progDesc "Scans a header file for extern function declarations.")
  where
    parser :: Opt.Parser CmdArgs
    parser = do
        project_root <- Opt.strOption (Opt.long "project-root" <> Opt.metavar "DIR" <> Opt.help "root directory of the project")
        header_file_path <- Opt.strOption (Opt.long "header-file-path" <> Opt.metavar "PATH" <> Opt.help "path to header file to scan")
        architecture <- Opt.strOption (Opt.long "architecture" <> Opt.metavar "ARCH" <> Opt.help "project architecture")
        debug <- Opt.switch (Opt.long "debug" <> Opt.help "Dump some useful debugging information.")
        pure CmdArgs {..}
