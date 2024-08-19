{
	nixpkgs ? import <nixpkgs> { },
	pkgs 		? nixpkgs.pkgsi686Linux,
}: with pkgs;
mkShell {
	buildInputs = [
		glibc
		clang
	];
}
